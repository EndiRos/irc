/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:00:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/19 12:32:14 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "network.hpp"
#include "users.hpp"

    
void Network::setup_socket()
{
    int flags_;
    int fdflags_;
    int yes_;
    
    yes_= 1;
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
        throw Err::make("socket failed", errno);

    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes_, sizeof(yes_)) == -1) {
        int e = errno;
        close(fd_);
        throw Err::make("setsockopt SO_REUSEADDR failed", e);
    }

    flags_ = fcntl(fd_, F_GETFL, 0);
    if (flags_ == -1 || fcntl(fd_, F_SETFL, flags_ | O_NONBLOCK) == -1) {
        int e = errno;
        close(fd_);
        throw Err::make("fcntl O_NONBLOCK failed", e);
    }

    fdflags_ = fcntl(fd_, F_GETFD, 0);
    if (fdflags_ != -1)
        (void)fcntl(fd_, F_SETFD, fdflags_ | FD_CLOEXEC);
    
    
    std::memset(&addr_, 0, sizeof(addr_));
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
    std::memset(&ev_, 0, sizeof(ev_));
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

void Network::epoll_run(User users) 
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
            if (fd == fd_ && (evs & EPOLLIN)) 
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
                        std::cerr << "read failed fd=" << fd << ": " << std::strerror(errno) << '\n';
                        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf_.erase(fd);
                        authed_.erase(fd);
                        break;
                    }
                    process_line(fd, inbuf_[fd]);
                    // Procesar líneas completas
                }
                    // Si cerramos dentro, salir del bucle de lectura
                if (inbuf_.find(fd) == inbuf_.end())
                    break;
                    // Si no hay más datos (EAGAIN se detecta arriba), salimos
                    // (el while(true) rompe por break en read)
                    // Continuará con más eventos epoll
                    break;
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
        std::memset(&cev, 0, sizeof(cev));
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
        // Mensaje inicial opcional
        send_small(cfd, ":server NOTICE * :Send password (plain or 'PASS <pwd>')\r\n");
    }
}

void Network::process_line(int fd, std::string& ib )
{
    std::string::size_type pos;
    while ((pos = ib.find('\n')) != std::string::npos) 
    {
        std::string line = ib.substr(0, pos);
        if (!line.empty() && line[line.size()-1] == '\r')
            line.erase(line.size()-1);
        ib.erase(0, pos + 1);
        if (line.substr(0,3) == "CAP")
            break;
        if (!authed_[fd]) 
        {
            // Password esperado
            authentificate(fd, line); 
        }
        if else (authed_[fd] && ) 
        else 
        {
            //aqui viene la busqueda de comandos, ejecutarlos  
            // Ya autenticado: procesar otros comandos
            std::cout << "fd " << fd << " <= " << line << '\n';
            send_small(fd, ":server NOTICE * :You said: " + line + "\r\n");
        }
    }
}

void Network::authentificate(int fd, std::string &candidate)
{
    // Soporta formato PASS <pwd>
    if (candidate.size() >= 5) 
    {
        //limpia string de llegada
        std::string up = candidate;
        for (size_t k=0;k < up.size() && k <4 ;++k)
            up[k] = (char)std::toupper((unsigned char)up[k]);
        if (up.compare(0,4,"PASS")==0) 
        {
            // Quitar "PASS"
            candidate.erase(0,4);
            // Trim espacios iniciales
            while (!candidate.empty() && (candidate[0]==' '||candidate[0]=='\t'))
                candidate.erase(0,1);
        }
    }
    if (candidate == pass_) //comprueba password server
    {
        authed_[fd] = true;
        tmp_user_->set_authen(true);
        send_small(fd, ":server NOTICE * :Password OK\r\n");
        // Aquí podrías continuar con NICK/USER, etc.
    } 
    else 
    {
        //falllo password envia mensaje y cierra conexion  
        send_small(fd, ":server NOTICE * :Bad password\r\n");
        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
        close(fd);
        inbuf_.erase(fd);
        authed_.erase(fd);
        delete tmp_user_;
        return; // salir de while líneas
    }
}

Network::Network(uint16_t port, std::string password):port_ (port), pass_(password)
{
    User *tmp_user_ = new User();
    server_ip_ = pick_ipv4();  
    setup_socket();
    bind_socket();
    epoll_setup();
    epoll_run();
}

Network::~Network(){}
    

