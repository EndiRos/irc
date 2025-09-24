#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdint.h>        // C++98: uint16_t
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <string>
#include "vector"

typedef struct user
{
    std::string nick;
    std::string name;
    u_int16_t   port;
    char        ip[INET_ADDRSTRLEN];
    std::vector<std::string> op; 
};

int create_listen_soccket (uint16_t port){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        std::cerr << "socket failed: " << std::strerror(errno) << '\n';
        return -1;
    }

    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        std::cerr << "setsockopt SO_REUSEADDR failed: " <<  std::strerror(errno) << '\n';
        close(fd);
        return -1;
    }

    // No bloqueante
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1){
        std::cerr << "fcntl O_NONBLOCK failed: " << std::strerror(errno) << '\n';
        close(fd);
        return -1;
    }
    // CLOEXEC
    int fdflags = fcntl(fd, F_GETFD, 0);
    if (fdflags != -1){
        (void)fcntl(fd, F_SETFD, fdflags | FD_CLOEXEC);
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1){
        std::cerr << "bind failed: " << std::strerror(errno) << '\n';
        close(fd);
        return -1;
    }
    if (listen(fd, SOMAXCONN) == -1)
    {
        std::cerr << "listen failed: " << std::strerror(errno) << '\n';
        close(fd);
        return -1;
    }
    return fd;
}

int main() {
    // Buffers por conexión
    std::map<int, std::string> inbuf;
    std::map<int, std::string> outbuf;
    std::map<std::string, std::vector<int>> chanels;
    std::map<int, user> users;

    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) {
        std::cerr << "epoll_create1 failed: " << std::strerror(errno) << '\n';
        return 1;
    }

    int listen_fd = create_listen_soccket(8080);
    if (listen_fd == -1) {
        close(epfd);
        return 1;
    }

    epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
        std::cerr << "epoll_ctl ADD failed: " << std::strerror(errno) << '\n';
        close(listen_fd);
        close(epfd);
        return 1;
    }

    std::cout << "Listening on 0.0.0.0:8080\n";

    const int MAX_EVENTS = 64;
    epoll_event events[MAX_EVENTS];

    for(;;){
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n == -1) {
            if (errno == EINTR) continue;
            std::cerr << "epoll_wait failed: " << std::strerror(errno) << '\n';
            break;
        }

        for (int i = 0; i < n; ++i)
        {
            int fd = events[i].data.fd;
            uint32_t evs = events[i].events;

            if (evs & (EPOLLERR | EPOLLHUP)){
                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
                close(fd);
                inbuf.erase(fd);
                outbuf.erase(fd);
                continue;
            }

            if (fd == listen_fd && (evs & EPOLLIN)){
                // Aceptar en bucle hasta EAGAIN
                for (;;) {
                    sockaddr_in cli;
                    socklen_t clen = sizeof(cli);
                    int cfd = accept(listen_fd, reinterpret_cast<sockaddr*>(&cli), &clen);
                    if (cfd == -1){
                        if (errno == EINTR) continue;
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        std::cerr << "accept failed: " << std::strerror(errno) << '\n';
                        break;
                    }
                    // Cliente no bloqueante + CLOEXEC
                    int fl = fcntl(cfd, F_GETFL, 0);
                    if (fl != -1) (void)fcntl(cfd, F_SETFL, fl | O_NONBLOCK);
                    int fdfl = fcntl(cfd, F_GETFD, 0);
                    if (fdfl != -1) (void)fcntl(cfd, F_SETFD, fdfl | FD_CLOEXEC);

                    // IP:puerto
                    
                    char ip[INET_ADDRSTRLEN] = {0};
                    inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
                    uint16_t c_port = ntohs(cli.sin_port);

                    epoll_event cev;
                    std::memset(&cev, 0, sizeof(cev));
                    cev.events = EPOLLIN;
                    cev.data.fd = cfd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cev) == -1){
                        std::cerr <<"epoll_ctl ADD client failed: " << std::strerror(errno) << '\n';
                        close(cfd);
                    } else {
                        std::cout << "new client fd=" << cfd << " [" << ip << ":" << c_port << "]\n";
                        
                        users[cfd].ip = ip;
                        users[cfd].port = c_port;

                        inbuf[cfd] = "";
                        outbuf[cfd] = "";
                    }
                }
                continue;
            }

            if (evs & EPOLLIN)
            {
                // Leer todo lo disponible
                for(;;){
                    char buf[4096];
                    ssize_t r = read(fd, buf, sizeof buf);
                    if (r > 0) {
                        inbuf[fd].append(buf, r);
                    } else if (r == 0) {
                        std::cout << "close client fd = " << fd << '\n';
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf.erase(fd);
                        outbuf.erase(fd);
                        break;
                    } else {
                        if (errno == EINTR) continue;
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        std::cerr << "read failed fd=" << fd << ": " << std::strerror(errno) << '\n';
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf.erase(fd);
                        outbuf.erase(fd);
                        break;
                    }
                }

                // Procesar líneas completas y preparar respuesta
                std::string &ib = inbuf[fd];
                std::string &ob = outbuf[fd];
                std::string::size_type pos;
                while ((pos = ib.find('\n')) != std::string::npos) {
                    std::string line = ib.substr(0, pos);
                    if (!line.empty() && line[line.size()-1] == '\r') line.erase(line.size()-1);

                    // aqui el parseo de lo recivido   
                    std::cout << "fd " << fd << " <= " << line << '\n';
                    ob += "You said: " + line + "\r\n";

                    ib.erase(0, pos + 1); 
                }

                // Intentar enviar lo pendiente inmediatamente
                if (!ob.empty()) {
                    ssize_t w = write(fd, ob.c_str(), ob.size());
                    if (w > 0) ob.erase(0, w);
                    if (!ob.empty()) {
                        // activar EPOLLOUT
                        epoll_event mod;
                        std::memset(&mod, 0, sizeof(mod));
                        mod.events = EPOLLIN | EPOLLOUT;
                        mod.data.fd = fd;
                        (void)epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &mod);
                    }
                }
            }

            if (evs & EPOLLOUT)
            {
                // Drenar buffer de salida
                std::string &ob = outbuf[fd];
                while (!ob.empty()) {
                    ssize_t w = write(fd, ob.c_str(), ob.size());
                    if (w > 0) {
                        ob.erase(0, w);
                    } else {
                        if (errno == EINTR) continue;
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        std::cerr << "write failed fd=" << fd << ": " << std::strerror(errno) << '\n';
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
                        close(fd);
                        inbuf.erase(fd);
                        outbuf.erase(fd);
                        break;
                    }
                }
                // Si ya no hay pendiente, volver a solo EPOLLIN
                if (outbuf.find(fd) != outbuf.end() && outbuf[fd].empty()) {
                    epoll_event mod;
                    std::memset(&mod, 0, sizeof(mod));
                    mod.events = EPOLLIN;
                    mod.data.fd = fd;
                    (void)epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &mod);
                }
            }
        }
    }

    close(listen_fd);
    close(epfd);
    return 0;
}