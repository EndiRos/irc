/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 09:06:47 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/30 09:10:30 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kick.hpp"

void kick(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
{
	msg_ res;
	size_t pos = msg.find(' ') + 1;
	size_t pos2 = msg.find(':');
	std::string channel_name = msg.substr(pos, msg.find(' ', pos) - pos);
	std::string user_name;
	std::string reason = ":";
	if (pos2 == std::string::npos)
	{
		user_name = msg.substr(msg.find(' ',pos) + 1,msg.find('\r') - msg.find(' ',pos) - 1);
		reason += "no reason";
	}
	else
	{
		user_name = msg.substr(msg.find(' ',pos) + 1, pos2 - msg.find(' ',pos) - 2);
		reason += msg.substr(pos2 + 1,  msg.find('\r') - pos2 - 1);
	}
	if (channels_list[channel_name].operators.find(user.get_nick()) == channels_list[channel_name].operators.end())
		return ;
	std::map<std::string,User>::iterator it = channels_list[channel_name].users.find(user_name);
    if (it!=channels_list[channel_name].users.end())
	{
		std::map<std::string,Channel>::iterator it2 = channels_list.find(channel_name);
		if (it2!=channels_list.end())
		{
			if (user.get_name() == "")
				user.set_name(user.get_nick());
			channels_list[channel_name].remove_user(user_list[user_name]);
			//res.user="Error:";
			res.channel = channel_name;
			res.user = ":"+user.get_nick()+"!"+user.get_name()+"@"+user.get_ip()+" KICK "+channel_name+" "+ user_name +" " + reason + "\r\n";
			//DOTS + nickname + TILD + username + IP_ADD + SPACE + command + SPACE + chan_name + SPACE + kcked_nick + " :" + nickname + NEW_LINE"
			std::cout << res.user << " fd: " << user_list[user_name].get_fd() << std::endl;
			Commands::send_to_one(user_list[user_name].get_fd(), res);
		}
	}
	Commands::refresh_users(user, channels_list, channel_name);
	return;
}
