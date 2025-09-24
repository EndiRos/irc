/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:31:48 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/24 14:08:17 by enetxeba         ###   ########.fr       */
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

#include "users.hpp"
#include "utils.hpp"

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
   // std::map<std::string, Chanels>  channels;

        User *tmp_user_;
    

        struct Err {
            static std::runtime_error make(const char *msg, int e) {
                std::string s(msg);
                if (e) {
                    std::string num;
                    int n = e;
                    if (n == 0)
                        num = "0";
                    else {
                        bool neg = false;
                        if (n < 0) { neg = true; n = -n; }
                        while (n > 0) {
                            num.insert(num.begin(), '0' + (n % 10));
                            n /= 10;
                        }
                        if (neg) num.insert(num.begin(), '-');
}
                    s += num;
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
        bool authentificate(std::string candidate);
        void new_user();
        void clean_msg(std::string& ib);
    public:
        Network(uint16_t port, std::string password);
        ~Network();
};
