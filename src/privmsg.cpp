/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:43:23 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/28 09:55:37 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "privmsg.hpp"
void privmsg(std::string msg, User user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &users)
{
    msg_ res;
    std::string::size_type pos =  msg.find(' ') + 1;
    std::string::size_type pos2 = msg.find(' ',pos);
    std::string::size_type pos_msg = msg.find(':') + 1;
   
    std::string send_to = msg.substr(pos, pos2 - pos);
    std::string msg_to_send = msg.substr(pos_msg);
    if (send_to[0] != '#'){
        res.user = msg_to_send;
        std::map<std::string, User>::iterator find_user = users.find(send_to);
        if (find_user == users.end())
            return;
        res.user = ":"+user.get_nick()+"!"+user.get_name()+"@"+user.get_ip()+" PRIVMSG "+send_to+" :"+msg_to_send+ "\r\n";
        Commands::send_to_one(find_user->second.get_fd(),res);
    }
    else{
        res.channel = send_to;
        res.all_user = ":"+user.get_nick()+"!"+user.get_name()+"@"+user.get_ip()+" PRIVMSG "+send_to+" :"+msg_to_send+ "\r\n";
        Commands::send_to_all(user.get_fd(),channels_list,res);
    }
    return;
}
