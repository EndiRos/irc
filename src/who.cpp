/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:07:08 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/30 09:11:12 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "who.hpp"

void who(std::string msg, User &user, std::map<std::string,Channel> &channels)
{
    std::string::size_type pos =  msg.find(' ') + 1;
    std::string::size_type pos2 = msg.find('\r',pos);
    std::string re_channel = msg.substr(pos,pos2 - pos);
    std::map<std::string,Channel>::iterator it = channels.find(re_channel);
    msg_ res;
    if (it == channels.end())
        return ;
    res.user += ":server 352 " + user.get_nick() + " = " + re_channel + " :" + channels[re_channel].user_list() + "\r\n";
    res.user += ":server 315 " + user.get_nick() + " " +  re_channel + " :End of /WHO list.\r\n";
    Commands::send_to_one(user.get_fd(),res);
    return;
}