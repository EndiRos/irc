/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:09:09 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/29 13:27:44 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"
#include "users.hpp"
#include "utils.hpp"


Commands::Commands() : commnad_len_(24) {
    const char* cmds[] = {
        "PASS", "NICK", "USER", "QUIT", "PING", "CAP", "PRIVMSG", "NOTICE",
        "JOIN", "PART", "TOPIC", "NAMES", "LIST", "MODE", "INVITE", "KICK",
        "WHOIS", "WHO", "ISON", "USERHOST", "AWAY", "OPER", "KILL", "KILL"
    };
    for (int i = 0; i < commnad_len_; ++i)
        comands_name_[i] = cmds[i];
}

void Commands::execute(std::string &msg, User& user, std::map<std::string, User> &user_list, std::map<std::string, User> &channels_list)
{
    (void)user;
    (void)user_list;//temporal ya lño usaremos 
    (void)channels_list;
    std::string::size_type pos = 0;
    std::string::size_type pos2 = 0; 
    pos2 = msg.find(' ');
    int i = 0;
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
    default:
        break;
    }
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