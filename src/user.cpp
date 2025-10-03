/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:45:28 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/03 09:18:32 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <map>
#include <arpa/inet.h>
#include <users.hpp>

User::User()
{
    user_.authen = false;
}
User& User::operator=(const User &other)
{
    if (this != &other)
    {
        set_authen(other.get_authen());
        set_fd(other.get_fd());
        set_nick(other.get_nick());
        set_name(other.get_name());
        set_real_name(other.get_real_name());
        set_ip(other.get_ip());
        set_port(other.get_port());
    }
    return *this;
}
void User::set_nick(std::string nick) {
    user_.nick = nick;
}
void User::set_authen(bool act) {
    user_.authen = act;
};
void User::set_name(std::string name){
    user_.name = name;
}
void User::set_real_name(std::string name){
    user_.name = name;
}
void User::set_ip(std::string ip){
    user_.ip = ip;
}
void User::set_port(uint16_t port){
    user_.port=port;
}
std::string User::get_nick() const
{
    return user_.nick;
}

void User::set_fd(int fd)
{
    user_.fd = fd ;
}
int User::get_fd() const
{
    return user_.fd;
}

bool User::get_authen() const {return user_.authen;}

std::string User::get_name() const {return user_.name;}
std::string User::get_real_name() const {return user_.real_name;}
std::string User::get_ip() const {return user_.ip;}
uint16_t User::get_port() const {return user_.port;}
void User::rm_channel(){}
User::~User() {};
