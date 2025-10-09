/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:00:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/09 11:49:55 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "network.hpp"
#include "users.hpp"
#include "commands.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>


void print_log(const std::string& msg) {
    std::time_t now = std::time(0);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::cout << "[" << buf << "] " << msg << std::endl;
}
#include <ctime>
#include <iomanip>
#include <iostream>


void print_log(const std::string& msg) {
    std::time_t now = std::time(0);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::cout << "[" << buf << "] " << msg << std::endl;
}

    
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
    //std::cout << "Listening on " << server_ip_ <<  " : "<< port_ << std::endl;
    //std::cout << "Listening on " << server_ip_ <<  " : "<< port_ << std::endl;
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
                        //msg_ res = quit(*tmp_user_, user_list);
                        //Commands::send_to_one(fd,res);
                        //std::cout << "close client fd = " << fd << '\n';
                        //msg_ res = quit(*tmp_user_, user_list);
                        //Commands::send_to_one(fd,res);
                        //std::cout << "close client fd = " << fd << '\n';
                        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf_.erase(fd);
                        authed_.erase(fd);
                        user_out(fd);
                        break;
                    } 
                    else 
                    {
                        if (errno == EINTR) continue;
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        std::cerr << "read failed fd=" << fd << ": " ": error code " << errno << '\n';
                        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                        msg_ msg;
                        msg.user =  "read failed";
                        Commands::send_to_one (fd, msg);
                        close(fd);
                        inbuf_.erase(fd);
                        authed_.erase(fd);
                        break;
                    }
                    print_log(inbuf_[fd]);
                    print_log(inbuf_[fd]);
                    process_line(fd, inbuf_[fd]);
                    inbuf_[fd].clear();
                    // Procesar líneas completas
                }
                    // Si cerramos dentro, salir del bucle de lectura
                if (inbuf_.find(fd) == inbuf_.end())
                    break;
                }
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
        //std::cout << "new client fd=" << cfd << " [" << ip << ":" << c_port << "]\n";
        //std::cout << "new client fd=" << cfd << " [" << ip << ":" << c_port << "]\n";
        inbuf_[cfd] = "";
        authed_[cfd] = false;
        tmp_user_= new User();
        tmp_user_->set_ip(ip);
        tmp_user_->set_port(c_port);
        tmp_user_->set_fd(cfd);
        //send_small(cfd, ":server NOTICE * :Send password (plain or 'PASS <pwd>')\r\n");
        //send_small(cfd, ":server NOTICE * :Send password (plain or 'PASS <pwd>')\r\n");
    }
}

bool Network::verify_cap(int fd, std::string& ib)
{
    int count = 0;
    int pos = 0;
    msg_ msg;
    while (true)
    {
        std::string::size_type first = ib.find('\n',pos);
        if (first == std::string::npos)
            break;
        count++;
        pos += first;
    }
    if (count == 2 && ib.substr(0,3) == "CAP")
    {    
        msg.user=":server CAP * LS :";
        Commands::send_to_one(fd, msg);
        return true;
    }
    return false;
}

void Network::process_line(int fd, std::string& ib )
{
    std::string res;
    //std::cout << ib << std::endl;
    //std::cout << ib << std::endl;
    std::string::size_type first = ib.find('\n'); //comprueba que la linea tiene una linea finaliza en \n
    if (first == std::string::npos)
        return;
    std::string::size_type second = ib.find('\n', first + 1); //si mas de una linea 
    if (second == std::string::npos && ib.substr(0,3) == "CAP")
        return;
    if (verify_cap(fd, ib))
        return;
    if (!authed_[fd]) 
    {
        // Password esperado
        if (com->authorize(ib,*tmp_user_, pass_, user_list))
        {
            msg_ res;
            res.user = ":server NOTICE * :Bad password\r\n";
            Commands::send_to_one(fd,res);
            msg_ res;
            res.user = ":server NOTICE * :Bad password\r\n";
            Commands::send_to_one(fd,res);
            epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
            close(fd);
            inbuf_.erase(fd);
            authed_.erase(fd);
            delete tmp_user_;
            return; // salir de while líneas
        }
        authed_[fd] = true;
        msg_ res;
        res.user = "Wellcome to server " + tmp_user_->get_nick() + "\r\n";
        Commands::send_to_one(fd,res);  //se puede personalizar
        msg_ res;
        res.user = "Wellcome to server " + tmp_user_->get_nick() + "\r\n";
        Commands::send_to_one(fd,res);  //se puede personalizar
        delete tmp_user_;
    }
    else 
    {
        User user = find_user_by_fd(fd);
        com->execute(ib,user, user_list, channels); 
        std::string look = ib;
        clean_msg(look);
    }
        
    }

    User Network::find_user_by_fd(int fd)
    {
        std::map<std::string, User>::iterator it = user_list.begin();
       ;
        for (; it != user_list.end(); ++it) {
        if (it->second.get_fd() == fd)
            return user_list[it->second.get_nick()];
        }
        return User();
    }

    void Network::user_out(int fd)
    {
        std::map<std::string, User>::iterator it_start = user_list.begin();
        std::map<std::string, User>::iterator it_end = user_list.end();
        for (; it_start != it_end; ++it_start)
        {
            if (it_start->second.get_fd() == fd)
            {
                it_start->second.set_fd(0);
                break;
            }
        }
    
}


Network::Network(uint16_t port, std::string password):port_ (port), pass_(password)
{
    com = new Commands();
    server_ip_ = pick_ipv4();  
    setup_socket();
    bind_socket();
    epoll_setup();
    epoll_run();
    delete com;
}

Network::~Network(){}



