/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imugica- <imugica-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:09:09 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/02 13:21:55 by imugica-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"
#include "users.hpp"
#include "utils.hpp"
#include <unistd.h>


Commands::Commands() : commnad_len_(24) {
    const char* cmds[] = {
        "PASS", "NICK", "USER", "QUIT", "PING", "CAP", "PRIVMSG", "NOTICE",
        "JOIN", "PART", "TOPIC", "NAMES", "LIST", "MODE", "INVITE", "KICK",
        "WHOIS", "WHO", "ISON", "USERHOST", "AWAY", "OPER", "KILL", "KILL"
    };
    for (int i = 0; i < commnad_len_; ++i)
        comands_name_[i] = cmds[i];
}



std::string mode(std::string &msg, User& user, std::map<std::string, Channel> &channels_list)
{
    (void)user;
	std::string channel = "";
	bool mode;
	size_t pos = msg.find('#');
	pos++;
	while (msg[pos] != ' ' && msg[pos] != '+' && msg[pos] != '-' && msg[pos])
	{
		channel += (char)msg[pos];
		pos++;
	}
	std::cout << channel << std::endl;
	while (msg[pos] == ' ')
		pos++;
	msg.erase(0, pos);
	pos = 0;
    std::string res;
	while(msg[pos] && msg[pos] != ' ')
	{
		std::cout << msg << std::endl;
		if(msg[pos] == '+')
		{
			mode = true;
			std::cout << "true mode" << std::endl;
		}
		else if (msg[pos] == '-')
		{
			mode = false;
			std::cout << "false mode" << std::endl;
		}
		switch (msg[pos])
		{
			case 'i':
				res += execute_i(mode, channels_list[channel]);
				break;
			case 't':
				res += execute_t(mode, channels_list[channel]);
				break;
			case 'k':
				res += execute_k(msg, mode, channels_list[channel]);
				break;
			case 'o':
				res += execute_o(msg, mode, channels_list[channel]);
				break;
			case 'l':
				res += execute_l(msg, mode, channels_list[channel]);
				break;
			default:
				break ;
		}
		msg.erase(0,1);
	}
    return res;
}


void Commands::execute(std::string &msg, User& user, std::map<std::string, User> &user_list, std::map<std::string, Channel> &channels_list)
{
    (void)user_list;//temporal ya lño usaremos 
    (void)channels_list;
    std::string::size_type pos = 0;
    std::string::size_type pos2 = 0;
    std::string channel = extract_channel(msg);
    pos2 = msg.find(' ');
    int i = 0;
    std::string res;
    for (; i < commnad_len_; i++ )
    {
        if (msg.substr(pos,pos2) == comands_name_[i])
            break;
    }
    switch (i)
    {
    case 0:
        break;
    case 1:
        //nick(line, user);
        break;
    case 13:
        res = mode(msg, user, channels_list);
        break;
    case 14:
        res = invite(msg, user, channels_list, user_list);
        break;
    case 8:
        res = join(msg,user,channels_list);
        break;
    default:
        break;
    }
    if (res.find("Error:") != 0)
        send_to_one(user.get_fd(), res);
    else
        send_to_one(user.get_fd(), res);
    return;
}

bool Commands::authorize(std::string &msg, User &tmp_user_, std::string pass,std::map<std::string, User> &user_list)
{   
    std::string::size_type pos = 0;
    while ( (pos = msg.find('\n')) != std::string::npos) 
    {
        std::string line = msg.substr(0, pos);
        if (!line.empty() && line[line.size()-1] == '\r')
            line.erase(line.size()-1);
        msg.erase(0, pos + 1);
        if (line.substr(0,3) == "CAP")
            continue;// Soporta formato PASS <pwd>
        else if (line.substr(0,4) == "PASS")
        {
            std::string msg_header = trim_msg(line,4);
            if (msg_header != pass)
                return (1);
            tmp_user_.set_authen(true);
        }
        else if (line.substr(0,4) == "NICK")
            tmp_user_.set_nick( trim_msg(line,4));
        else if (line.substr(0,4) == "USER")
       {
            tmp_user_.set_name(trim_msg(line, 4, ' '));
            line.erase(0,line.find(':'));
            tmp_user_.set_real_name(trim_msg(line,line.find('\n')));
       }
    }
    add_user(tmp_user_, user_list);
    return (0);
}
Commands::~Commands(){}

void Commands::add_user(User &tmp_user, std::map<std::string, User> &user_list)
{
    std::map<std::string, User>::iterator it = user_list.find(tmp_user.get_nick());
    if (it ==user_list.end())
    {
        user_list[tmp_user.get_nick()].set_authen(tmp_user.get_authen());
        user_list[tmp_user.get_nick()].set_nick(tmp_user.get_nick());
        user_list[tmp_user.get_nick()].set_name(tmp_user.get_name());
        user_list[tmp_user.get_nick()].set_real_name(tmp_user.get_real_name());
        user_list[tmp_user.get_nick()].set_ip(tmp_user.get_ip());
        user_list[tmp_user.get_nick()].set_port(tmp_user.get_port());
        std::cout << "New user: " << tmp_user.get_nick() << " enter on server" << std::endl;
        
    }
    else 
    {
        std::cout << "Existing user: " << tmp_user.get_nick() << "enter on server" << std::endl;
        user_list[tmp_user.get_nick()].set_fd (tmp_user.get_fd());
    }
}

void Commands::send_to_one(int fd, std::string msg)
{
    write (fd,msg.c_str(),msg.size());
}