/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/10/13 12:38:58 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "commands.hpp"

Commands::Commands() : commnad_len_(24) {
    const char* cmds[] = {
        "PASS", "NICK", "USER", "QUIT", "PING", "CAP", "PRIVMSG", "NOTICE",
        "JOIN", "PART", "TOPIC", "NAMES", "LIST", "MODE", "INVITE", "KICK",
        "WHOIS", "WHO", "ISON", "USERHOST", "AWAY", "OPER", "KILL", "HOW"
    };
    for (int i = 0; i < commnad_len_; ++i)
        comands_name_[i] = cmds[i];
}


void Commands::execute(std::string &msg, User& user, std::map<std::string, User> &user_list, std::map<std::string, Channel> &channels_list)
{
    (void)user_list;//temporal ya lño usaremos 
    (void)channels_list;
    msg_ res;
    std::string::size_type pos = 0;
    std::string::size_type pos2 = 0;
    res.channel = extract_channel(msg);
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
    case 3:
        quit(user, user_list);
        break;
    case 10:
        res = topic(msg, user, channels_list,user_list);
        break;
    case 13:
        res = mode(msg, user, channels_list,res);
        break;
    case 15:
        res = kick(msg, user, channels_list,user_list);
        break;
    case 8:
        join_chanel(msg,user,channels_list);
        break;
    case 24:
        res = who(msg,user,channels_list);
    default:
        break;
    }
    send_to_one(user.get_fd(), res);
    send_to_all(user.get_fd(),channels_list, res);
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

    if (tmp_user_.get_nick() == ""){
        return(1);
    }
    add_user(tmp_user_, user_list);
    return (0);
}
Commands::~Commands(){}

void Commands::add_user(User &tmp_user, std::map<std::string, User> &user_list)
{
    std::map<std::string, User>::iterator it = user_list.find(tmp_user.get_nick());
    if (it == user_list.end())
    {
        user_list[tmp_user.get_nick()] = tmp_user;
        std::cout << "New user: " << tmp_user.get_nick() << " enter on server" << std::endl;
        
    }
    else 
    {
        std::cout << "Existing user: " << tmp_user.get_nick() << "enter on server" << std::endl;
        user_list[tmp_user.get_nick()].set_fd (tmp_user.get_fd());
    }
}

void Commands::send_to_one(int fd, msg_ msg)
{
    write (fd,msg.user.c_str(),msg.user.size());
}
void Commands::send_to_all(int fd, std::map<std::string,Channel> chanel_list, msg_  msg)
{
    std::map<std::string,Channel>::iterator chan_search = chanel_list.find(msg.channel);
    if (chan_search == chanel_list.end()) 
        return;
    std::map<std::string,User>::iterator user_it = chanel_list[msg.channel].users.begin();
    for (;user_it != chanel_list[msg.channel].users.end(); ++user_it)
    {
        if (user_it->second.get_fd() != fd &&  user_it->second.get_fd() != 0)
            write (user_it->second.get_fd(),msg.all_user.c_str(), msg.all_user.size());
    }
}
void Commands::refresh_users(User &user,std::map<std::string,Channel> &channels, std::string &channel)
{
    std::map<std::string,User>::iterator users = channels[channel].users.begin();
    std::map<std::string,User>::iterator users_end = channels[channel].users.end();
    std::map<std::string,User> user_list= channels[channel].users;
    
    if (users != users_end && user_list.find(user.get_nick()) != user_list.end())
    {
        for(; users != users_end; ++users)
        {
        if (users->second.get_fd() == 0)
            continue;
        msg_ res;
        res.user += ":server 353 " + users->second.get_nick() + " = " + channel + " :" + channels[channel].user_list() + "\r\n";
        res.user += ":server 366 " + users->second.get_nick() + " " + channel + " :End of /NAMES list.\r\n";
        send_to_one(users->second.get_fd(),res);
        }
    }
    else
    {
        msg_ res;
        res.user += ":server 353 " + user.get_nick() + " = " + channel + " :" + channels[channel].user_list() + "\r\n";
        res.user += ":server 366 " + user.get_nick() + " " + channel + " :End of /NAMES list.\r\n";
        send_to_one(user.get_fd(),res);
    }
}