/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:07:08 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/14 11:12:32 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "join.hpp"

void join_chanel(std::string msg, User &user, std::map<std::string,Channel> &channels)
{
    std::string::size_type pos =  msg.find(' ') + 1;
    std::string::size_type pos2 = msg.find('\r',pos);
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
        channels[re_channel].add_user(user);
        res.user = ":" + user.get_nick() + "!" + user.get_name() + "@" + user.get_ip() + " JOIN :" + re_channel + "\r\n";
        Commands::send_to_one(user.get_fd(),res);
    }
    Commands::refresh_users(user,channels, re_channel);
    return;
}

