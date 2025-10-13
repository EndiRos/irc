/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:31:48 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/13 11:21:16 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sys/epoll.h>
#include <iostream>
#include <net/if.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <map>
#include <stdlib.h>
#include <sstream>

#include "users.hpp"
#include "utils.hpp"
#include "commands.hpp"
#include "channel.hpp"
#include "quit.hpp"
#include "msg.hpp"
#include "quit.hpp"
#include "msg.hpp"

class Network
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
        std::map<std::string, User> user_list;
        std::map<std::string, Channel>  channels;

        User *tmp_user_;
        Commands *com;
        
            struct Err {
            static std::runtime_error make(const char *msg, int e) {
                std::string s(msg);
                if (e) {
                   
                    std::ostringstream num;
                    num<<e;
                    s += num.str();
                }
                return std::runtime_error(s);
            }
        };
        void setup_socket();
        std::string pick_ipv4();
        void bind_socket();
        void epoll_setup();
        void epoll_run();
        void new_connection();
        void process_line(int fd, std::string &ib);
        bool authentificate(std::string candidate);
        void new_user();
        User find_user_by_fd(int fd);
        void user_out(int fd);
        bool verify_cap(int fd, std::string& ib);
    public:
        Network(uint16_t port, std::string password);
        ~Network();
};
