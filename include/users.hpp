/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:12:59 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/03 09:19:16 by enetxeba         ###   ########.fr       */
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
        User& operator = (const User& other);
        void set_nick(std::string nick);
        void set_authen(bool act);
        void set_name(std::string name);
        void set_real_name(std::string name);
        void set_ip(std::string ip);
        void set_port(uint16_t port);
        void add_channel(std::string);
        void add_user();
        bool get_authen()const ;
        std::string get_nick() const ;
        std::string get_name()const ;
        std::string get_real_name() const;
        std::string get_ip()  const;
        uint16_t get_port()const;
        void rm_channel();
        void set_fd(int fd);
        int get_fd() const;
    
        static  std::map<std::string, data> users;
    
   
};