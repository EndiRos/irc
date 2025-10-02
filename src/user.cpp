/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imugica- <imugica-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:45:28 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/02 13:37:10 by imugica-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <map>
#include <users.hpp>
#include <vector>

User::User()
{
	user_.authen = false;
}
void User::set_nick(std::string nick)
{
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
	user_.name = name;
}
void User::set_ip(std::string ip)
{
	user_.ip = ip;
}
void User::set_port(uint16_t port)
{
	user_.port = port;
}
std::string User::get_nick()
{
	return (user_.nick);
}

void User::set_fd(int fd)
{
	user_.fd = fd;
}
int User::get_fd()
{
	return (user_.fd);
}

void User::add_invite( std::string channel)
{
    user_.invited_list.push_back(channel);
}

bool User::get_authen()
{
	return (user_.authen);
}

std::string User::get_name()
{
	return (user_.name);
}
std::string User::get_real_name()
{
	return (user_.real_name);
}
std::string User::get_ip()
{
	return (user_.ip);
}
uint16_t User::get_port()
{
	return (user_.port);
}
void User::rm_channel()
{
}
User::~User(){};
