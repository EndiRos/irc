/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:00:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/24 13:38:00 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "network.hpp"
#include "users.hpp"
#include "commands.hpp"

    
void Network::setup_socket()
{
    int flags_;
    int fdflags_;
    int yes_;
    
    yes_= 1;
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1){
        int e = errno;
        throw Err::make("socket failed",e);
    }

    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes_, sizeof(yes_)) == -1) {
        int e = errno;
        close(fd_);
        throw Err::make("setsockopt SO_REUSEADDR failed",e);
    }

    flags_ = fcntl(fd_, F_GETFL, 0);
    if (flags_ == -1 || fcntl(fd_, F_SETFL, flags_ | O_NONBLOCK) == -1) {
        close(fd_);
        int e = errno;
        throw Err::make("fcntl O_NONBLOCK failed",e);
    }

    fdflags_ = fcntl(fd_, F_GETFD, 0);
    if (fdflags_ != -1)
        (void)fcntl(fd_, F_SETFD, fdflags_ | FD_CLOEXEC);
    
    
    my_memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(port_);
}

void Network::bind_socket()
{
    if (bind(fd_, reinterpret_cast<sockaddr*>(&addr_), sizeof(addr_)) == -1)
    {
        int e = errno;
        close(fd_);
        throw Err::make("bind failed", e);
    }
    if (listen(fd_, SOMAXCONN) == -1)
    {
        int e = errno;
        close(fd_);
        throw Err::make("listen failed", e);
    }
}

std::string Network::pick_ipv4()
{
    struct ifaddrs *ifaddr = 0;
    if (getifaddrs(&ifaddr) == -1)
        return "127.0.0.1";
    std::string chosen = "127.0.0.1";
    for (struct ifaddrs *ifa = ifaddr; ifa; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr) continue;
        if (ifa->ifa_addr->sa_family != AF_INET) continue;
        // Evitar loopback
        if ((ifa->ifa_flags & IFF_LOOPBACK) != 0) continue;
        // Debe estar “UP” y “RUNNING”
        if ((ifa->ifa_flags & IFF_UP) == 0) continue;
        if ((ifa->ifa_flags & IFF_RUNNING) == 0) continue;
        char host[NI_MAXHOST];
        if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                        host, sizeof(host), 0, 0, NI_NUMERICHOST) == 0)
        {
            chosen = host;
            break;
        }
    }
    freeifaddrs(ifaddr);
    return chosen;
}

void Network::epoll_setup()
{
    epfd_ = epoll_create1(EPOLL_CLOEXEC);
    if (epfd_== -1)
    {
        int e = errno;
        throw Err::make("epoll_create1 failed", e);
    }
    if (fd_ == -1) 
    {
        close(epfd_);
        return ;
    }
    epoll_event ev_;
    my_memset(&ev_, 0, sizeof(ev_));
    ev_.events = EPOLLIN;
    ev_.data.fd = fd_;
    
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd_, &ev_) == -1)
    {
        int e = errno;
        throw Err::make("epoll_ctl ADD failed", e);
        close(fd_);
        close(epfd_);
        return ;
    }
    std::cout << "Listening on " << server_ip_ <<  " : "<< port_ << std::endl;
}

void Network::epoll_run() 
{
    const int MAX_EVENTS = 64;
    epoll_event events[MAX_EVENTS];
    for(;;) {
        int n = epoll_wait(epfd_, events, MAX_EVENTS, -1);
        if (n == -1) 
        {
            if (errno == EINTR) continue;
            int e = errno;
            throw Err::make("epoll_wait failed", e);
        }
        for (int i = 0; i < n; ++i) 
        {
            int fd = events[i].data.fd;
            uint32_t evs = events[i].events;
            if (evs & (EPOLLERR | EPOLLHUP)) 
            {
                epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                close(fd);
                inbuf_.erase(fd);
                authed_.erase(fd);
                continue;
            }
            // Aceptar nuevas conexiones
            if (fd == fd_ && (evs & EPOLLIN) ) 
            {
                new_connection();
                continue;
            }
            // Lectura de clientes
            if (evs & EPOLLIN) 
            {
                while (true) 
                {
                    char buf[4096];
                    ssize_t r = read(fd, buf, sizeof buf);
                    if (r > 0) {
                        inbuf_[fd].append(buf, r);
                    } 
                    else if (r == 0) 
                    {
                        std::cout << "close client fd = " << fd << '\n';
                        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf_.erase(fd);
                        authed_.erase(fd);
                        break;
                    } 
                    else 
                    {
                        if (errno == EINTR) continue;
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        std::cerr << "read failed fd=" << fd << ": " ": error code " << errno << '\n';
                        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf_.erase(fd);
                        authed_.erase(fd);
                        break;
                    }
                    process_line(fd, inbuf_[fd]);
                    inbuf_[fd].clear();
                    // Procesar líneas completas
                }
                    // Si cerramos dentro, salir del bucle de lectura
                if (inbuf_.find(fd) == inbuf_.end())
                    break;
                    // Si no hay más datos (EAGAIN se detecta arriba), salimos
                    // (el while(true) rompe por break en read)
                    // Continuará con más eventos epoll
                //break;
                }
            }
        }
}

    
void Network::send_small(int fd, const std::string &msg) {
        const char *p = msg.c_str();
        size_t left = msg.size();
        while (left) {
            ssize_t w = write(fd, p, left);
            if (w > 0) { left -= (size_t)w; p += w; }
            else {
                if (w == -1 && (errno == EINTR)) continue;
                // Si EAGAIN dejamos el resto (mensaje corto normalmente cabe)
                break;
            }
        }
    }

void Network::new_connection()
{
    while (true) 
    {
        sockaddr_in cli;
        socklen_t clen = sizeof(cli);
        int cfd = accept(fd_, reinterpret_cast<sockaddr*>(&cli), &clen);
        if (cfd == -1) 
        {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            int e = errno;
            throw Err::make("accept failed", e);
        }
        //Comprueba que la conexion establecida es nonblock y si no la pone en non block 
        int fl = fcntl(cfd, F_GETFL, 0);
        if (fl != -1) fcntl(cfd, F_SETFL, fl | O_NONBLOCK);
        //Comprueba que lla conexion esta en closeexec para cierres automaticos
        int fdfl = fcntl(cfd, F_GETFD, 0);
        if (fdfl != -1) fcntl(cfd, F_SETFD, fdfl | FD_CLOEXEC);
        
        char ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
        uint16_t c_port = ntohs(cli.sin_port);
        epoll_event cev;
        my_memset(&cev, 0, sizeof(cev));
        cev.events = EPOLLIN;
        cev.data.fd = cfd;
        if (epoll_ctl(epfd_, EPOLL_CTL_ADD, cfd, &cev) == -1) {
            int e = errno;
            close(cfd);
            throw Err::make("epoll_ctl ADD client failed", e);
        }
        std::cout << "new client fd=" << cfd << " [" << ip << ":" << c_port << "]\n";
        inbuf_[cfd] = "";
        authed_[cfd] = false;
        tmp_user_= new User();
        tmp_user_->set_ip(ip);
        tmp_user_->set_port(c_port);
        // Mensaje inicial opcional
        send_small(cfd, ":server NOTICE * :Send password (plain or 'PASS <pwd>')\r\n");
    }
}

void Network::process_line(int fd, std::string& ib )
{
    std::string::size_type first = ib.find('\n');
    if (first == std::string::npos)
        return;
    std::string::size_type second = ib.find('\n', first + 1);
    if (second == std::string::npos && ib.substr(0,3) == "CAP")
        return;
    if (!authed_[fd]) 
    {
        // Password esperado
        if (authentificate(ib))
        {
            send_small(fd, ":server NOTICE * :Bad password\r\n");
            epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
            close(fd);
            inbuf_.erase(fd);
            authed_.erase(fd);
            delete tmp_user_;
            return; // salir de while líneas
        }
        std::map<std::string, User>::iterator it = user_list.find(tmp_user_->get_nick());
        if (it ==user_list.end())
        {
            user_list[tmp_user_->get_nick()].set_authen(tmp_user_->get_authen());
            user_list[tmp_user_->get_nick()].set_nick(tmp_user_->get_nick());
            user_list[tmp_user_->get_nick()].set_name(tmp_user_->get_name());
            user_list[tmp_user_->get_nick()].set_real_name(tmp_user_->get_real_name());
            user_list[tmp_user_->get_nick()].set_ip(tmp_user_->get_ip());
            user_list[tmp_user_->get_nick()].set_port(tmp_user_->get_port());
            std::cout << "New user: " << tmp_user_->get_nick() << " enter on server" << std::endl;
            authed_[fd] = true;
        }
        else 
        {
            std::cout << "Existing user: " << tmp_user_->get_nick() << "enter on server" << std::endl;
            authed_[fd] = true;
        }
        send_small(fd, "Wellcome to server " + tmp_user_->get_nick() + "\r\n");
        delete tmp_user_;
    }
    else 
    {
        //aqui viene la busqueda de comandos, ejecutarlos  
        // Ya autenticado: procesar otros comandos
        std::string look = ib;
        clean_msg(look);
        std::cout << look.substr(0,4) <<std::endl;
        if (look.substr(0,4) == "JOIN")
        {
            std::string msg = ":enetxeba!enetxeba@127.0.0.1 JOIN :#canal\r\n\
                            :server 353 enetxeba = #canal :juan\r\n\
                            :server 366 enetxeba #canal :End of /NAMES list.\r\n";
            send_small(fd,msg);
           return;
        }
        else 
        {
            std::cout << "fd " << fd << " <= " << ib << '\n';
            send_small(fd, ":server NOTICE * :You said: " + ib + "\r\n");
            return;
        }
    }
        
    }
void Network::clean_msg(std::string& ib)
{

    for (int i = 0; i < 4; i++)
    {
        std::string::size_type pos = ib.find('\n');
        if (pos == std::string::npos)
            break;
        ib.erase(0,pos + 1);
    }
}

bool Network::authentificate(std::string msg)
{
    
    if (auth_user(msg, *tmp_user_, pass_))
        return (1);
    return 0; 
}


Network::Network(uint16_t port, std::string password):port_ (port), pass_(password)
{
    server_ip_ = pick_ipv4();  
    setup_socket();
    bind_socket();
    epoll_setup();
    epoll_run();
}

Network::~Network(){}
    

