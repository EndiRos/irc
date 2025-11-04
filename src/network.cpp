/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:00:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/11/03 13:15:42 by enetxeba         ###   ########.fr       */
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

    
void Network::setup_socket()
{
    int yes_;
    
    yes_= 1;
    fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (fd_ == -1){
        throw Err::make("socket failed");
    }
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes_, sizeof(yes_)) == -1) {
        close(fd_);
        throw Err::make("setsockopt SO_REUSEADDR failed");
    }
    my_memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(port_);
}

void Network::bind_socket()
{
    if (bind(fd_, reinterpret_cast<sockaddr*>(&addr_), sizeof(addr_)) == -1)
    {
        close(fd_);
        throw Err::make("bind failed");
    }
    if (listen(fd_, SOMAXCONN) == -1)
    {
        close(fd_);
        throw Err::make("listen failed");
    }
}

void Network::epoll_setup()
{
    
    epfd_ = epoll_create1(0);
    if (epfd_== -1)
        throw Err::make("epoll_create1 failed");

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, 0)){
        throw Err::make ("sigpocmask failed");
    }
    signalfd_ = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
    if (signalfd_ == -1 )
        throw Err::make ("signal_fd failed");
    epoll_event sev;
    my_memset(&sev,0,sizeof(sev));
    sev.events = EPOLLIN;
    sev.data.fd = signalfd_;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, signalfd_, &sev) == -1){
        close (signalfd_);
        close (epfd_);
        throw Err::make ("epoll_ctl ADD signal failerd");
    }
    runnig_ = true;
    
   if (fd_ != -1) {
        epoll_event lev;
        my_memset(&lev, 0, sizeof(lev));
        lev.events = EPOLLIN;
        lev.data.fd = fd_;
        if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd_, &lev) == -1) {
            // limpiar recursos en caso de fallo
            epoll_ctl(epfd_, EPOLL_CTL_DEL, signalfd_, 0);
            close(signalfd_);
            close(epfd_);
            throw Err::make("epoll_ctl ADD listen fd failed");
        }
    }
}

void Network::epoll_run() 
{
    const int MAX_EVENTS = 64;
    epoll_event events[MAX_EVENTS];
    while (runnig_) {
        int n = epoll_wait(epfd_, events, MAX_EVENTS, -1);
        if (n == -1) 
        {   //if (errno == EINTR) continue;
            throw Err::make("epoll_wait failed");
        }
        for (int i = 0; i < n; ++i) 
        {
            int fd = events[i].data.fd;
            uint32_t evs = events[i].events;
            if (fd == signalfd_){
                struct signalfd_siginfo fdsi;
                ssize_t s = read(signalfd_, &fdsi, sizeof(fdsi));
                if (s == sizeof(fdsi)){
                    std::cout << "SIGINT received , close service \n";
                    runnig_ = false;
                    break;
                }
            }
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
                        continue;
                    }
                    if (r == 0) {
                        // peer cerró la conexión -> limpiar y salir
                        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf_.erase(fd);
                        authed_.erase(fd);
                        break;
                    }
                    // r < 0: tratamos como "no más datos ahora" (sin errno disponible)
                    // salir del bucle de lectura y procesar lo acumulado
                    break;
                } // end while read

                if (inbuf_.find(fd) == inbuf_.end())
                    continue;

                // procesar buffer acumulado
                print_log(inbuf_[fd]);
                process_line(fd, inbuf_[fd]);
                // no forzamos cerrar aquí; process_line puede modificar el buffer
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
        if (fd_ < 0) {
            std::cerr << "Error: fd_ inválido antes de accept" << std::endl;
            break;
        }

        int cfd = accept(fd_, reinterpret_cast<sockaddr*>(&cli), &clen);
        if (cfd == -1) {
            // No usamos errno aquí: salimos del bucle de aceptación
            break;
        }

        // Poner socket en non-blocking (solo F_SETFL con O_NONBLOCK, permitido)
        fcntl(cfd, F_SETFL, O_NONBLOCK);
       
        char ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
        uint16_t c_port = ntohs(cli.sin_port);

        epoll_event cev;
        my_memset(&cev, 0, sizeof(cev));
        cev.events = EPOLLIN;
        cev.data.fd = cfd;
        if (epoll_ctl(epfd_, EPOLL_CTL_ADD, cfd, &cev) == -1) {
            close(cfd);
            throw Err::make("epoll_ctl ADD client failed");
        }

        inbuf_[cfd] = "";
        authed_[cfd] = false;
        tmp_user_ = new User();
        tmp_user_->set_ip(ip);
        tmp_user_->set_port(c_port);
        tmp_user_->set_fd(cfd);
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
            epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0);
            close(fd);
            inbuf_.erase(fd);
            authed_.erase(fd);
            return; // salir de while líneas
        }
        authed_[fd] = true; 
        msg_ res;
        res.user = "Wellcome to server " + tmp_user_->get_nick() + "\r\n";
        Commands::send_to_one(fd,res);  //se puede personalizar
        tmp_user_->reset_user();
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
    setup_socket();
    bind_socket();
    epoll_setup();
    epoll_run();
    delete com;
}

Network::~Network(){}



