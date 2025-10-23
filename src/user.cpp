/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imugica- <imugica-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:45:28 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/23 11:17:41 by imugica-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <map>
#include <users.hpp>
#include <vector>
#include <algorithm>

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
void User::set_authen(bool act)
{
	user_.authen = act;
};
void User::set_name(std::string name)
{
	user_.name = name;
}
void User::set_real_name(std::string name)
{
	user_.real_name = name;
}
void User::set_ip(std::string ip)
{
	user_.ip = ip;
}
void User::set_port(uint16_t port)
{
	user_.port = port;
}
std::string User::get_nick() const
{
	return (user_.nick);
}

void User::set_fd(int fd)
{
	user_.fd = fd;
}
int User::get_fd() const
{
	return (user_.fd);
}

void User::add_invite( std::string channel)
{
    user_.invited_list.push_back(channel);
}
bool User::is_invited(std::string channel_name)
{
    std::vector<std::string> names = user_.invited_list;
    std::vector<std::string>::iterator it = std::find(names.begin(), names.end(), channel_name);

    if (it != names.end())
        return true;
    else
        return false;
}
void User::reset_user()
{
    user_.fd= 0;
    user_.port = 0;
    user_.ip = "";
    user_.name="";
    user_.nick="";
    user_.authen = false;
}

bool User::get_authen() const {return user_.authen;}

std::string User::get_name() const {return user_.name;}
std::string User::get_real_name() const {return user_.real_name;}
std::string User::get_ip() const {return user_.ip;}
uint16_t User::get_port() const {return user_.port;}
void User::rm_channel(){}
User::~User() {};
