/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:07:08 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/03 10:20:52 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "join.hpp"

msg_ join_chanel(std::string msg, User &user, std::map<std::string,Channel> &channels)
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
        channels[re_channel]=tmp_chan;
        res.user = ":"+ user.get_nick() + "@" + user.get_ip() + " JOIN :" + re_channel + "\r\n" ;
        res.user += ":server 353 "+ user.get_nick() + " = #" + re_channel +" :" + user.get_nick() + "\r\n" ;
        res.user += ":server 366 "+ user.get_nick() + " #" + re_channel +" :End of /NAMES list.\r\n" ;
    }
    return res;
}