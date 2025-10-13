/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:07:08 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/09 12:25:23 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "who.hpp"

msg_ who(std::string msg, User &user, std::map<std::string,Channel> &channels)
{
    std::string::size_type pos =  msg.find(' ') + 1;
    std::string::size_type pos2 = msg.find('\r',pos);
    std::string re_channel = msg.substr(pos,pos2 - pos);
    std::map<std::string,Channel>::iterator it=channels.find(re_channel);
    msg_ res;
    if (it==channels.end())
        return res;
    res.user += ":server 353 " + user.get_nick() + " = " + re_channel + " :" + channels[re_channel].user_list() + "\r\n";
    res.user += ":server 366 " + user.get_nick() + " " + re_channel + " :End of /NAMES list.\r\n";
    return res;
}
/* :server 352 <nick> <canal> <user> <host> <server> <nick> <H|G>[*@+] :<hopcount> <realname>
:server 315 <nick> <canal> :End of /WHO list.

":server 352 " + user.get_nick() + " = " + re_channel + " :" + channels[re_channel].user_list() + "\r\n";
 */