/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:07:08 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/27 10:22:54 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "join.hpp"

bool check_invite(Channel channel, User &user)
{
    if (channel.invite_f)
        return user.is_invited(channel.c_name);      
    else
        return true;
}

void join_chanel(std::string msg, User &user, std::map<std::string,Channel> &channels)
{
    std::string::size_type pos =  msg.find(' ') + 1;
    std::string::size_type pos2 = msg.find(' ',pos);
    std::string::size_type pos3 = msg.find('\r');
    if (pos2 == std::string::npos)
        pos2=pos3;
    std::string re_channel = msg.substr(pos,pos2 - pos);
    std::map<std::string,Channel>::iterator it=channels.find(re_channel);
    msg_ res;
    if (it==channels.end())
    {
        Channel tmp_chan(user,re_channel);
        tmp_chan.add_operators(user);
        tmp_chan.add_user(user);
        channels[re_channel]=tmp_chan;
        res.user = ":" + user.get_nick() + "!" + user.get_name() + "@" + user.get_ip() + " JOIN :" + re_channel + "\r\n";
        Commands::send_to_one(user.get_fd(),res);
    }
    else
    {
        if (!check_invite(channels[re_channel],user))
        {
            return ;
        }
        if (!channels[re_channel].key_f)
       { 
            channels[re_channel].add_user(user);
            res.user = ":" + user.get_nick() + "!" + user.get_name() + "@" + user.get_ip() + " JOIN :" + re_channel + "\r\n";
            Commands::send_to_one(user.get_fd(),res);
        }
        else{
            std::string re_key = msg.substr(pos2 + 1,pos3 - (pos2 + 1));
            if (re_key == channels[re_channel].key)
            {
                channels[re_channel].add_user(user);
                res.user = ":" + user.get_nick() + "!" + user.get_name() + "@" + user.get_ip() + " JOIN :" + re_channel + "\r\n";
                Commands::send_to_one(user.get_fd(),res);
            }
            else{
                res.user = ":server NOTICE " + user.get_name() + " :Password incorrect\r\n";
                Commands::send_to_one(user.get_fd(),res);
                return;
            }
        }
        
    }
    Commands::refresh_users(user,channels, re_channel);
    return;
}

