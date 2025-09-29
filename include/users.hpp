/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:12:59 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/29 11:51:24 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <arpa/inet.h>

struct data{
            std::string                 nick;
            std::string                 name;
            bool                        authen;
            std::string                 real_name;
            std::string                 ip;
            uint16_t                    port;
            int                         fd;
};
       
class User
{
    
    private:
        
        data user_;
    
    public:    
        User();
        ~User();
        void set_nick(std::string nick);
        void set_authen(bool act);
        void set_name(std::string name);
        void set_real_name(std::string name);
        void set_ip(std::string ip);
        void set_port(uint16_t port);
        void add_channel(std::string);
        void add_user();
        bool get_authen();
        std::string get_nick();
        std::string get_name();
        std::string get_real_name();
        std::string get_ip();
        uint16_t get_port();
        void rm_channel();
        void set_fd(int fd);
        int get_fd();
    
        static  std::map<std::string, data> users;
    
   
};