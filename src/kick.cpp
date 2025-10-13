#include <iostream>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"
#include "commands.hpp"

msg_  kick(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
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
			res.user = ":"+user.get_nick()+"!"+user.get_name()+"@"+user.get_ip()+" KICK #"+channel_name+" "+ user_name +" " + reason + "\r\n";
		}
	}
	Commands::refresh_users(channels_list, channel_name);
	return res;
}
