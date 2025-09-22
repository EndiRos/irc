/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:31:48 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/19 12:18:32 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/epoll.h>
#include <iostream>
#include <net/if.h>
#include <ifaddrs.h>
#include <netdb.h>
#include<map>

#include "users.hpp"

class Network: public User
{
    private:
    int fd_;
    int epfd_;
    uint16_t port_;
    sockaddr_in addr_;
    std::string server_ip_;
    std::string pass_;

    std::map<int, std::string> inbuf_;
    std::map<int, bool> authed_;

    User *tmp_user_;
    

    struct Err {
        static std::runtime_error make(const char *msg, int e) {
            std::string s(msg);
            if (e) {
                s += ": ";
                s += std::strerror(e);
            }
            return std::runtime_error(s);
        }
    };
        void setup_socket();
        std::string pick_ipv4();
        void bind_socket();
        void epoll_setup();
        void epoll_run();
        void send_small(int fd, const std::string &msg);
        void new_connection();
        void process_line(int fd, std::string &ib);
        void authentificate(int fd, std::string &candidate);
        void new_user();
    public:
        Network(uint16_t port, std::string password);
        ~Network();
};
