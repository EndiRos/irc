#include <iostream>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"

msg_  kick(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
{
	msg_ res;
	size_t pos = msg.find('#') + 1;
	size_t pos2 = msg.find(':') + 1;
	//1- find chhanel name
	std::string channel_name = msg.substr(pos, msg.find(' '));
	//2 -find user name 
	std::string user_name;
	if (pos2 == std::string::npos)
		user_name = msg.substr(msg.find(' ',pos),msg.size());
	else
		user_name = msg.substr(msg.find(' ',pos),pos2 - msg.find(' ', pos) - 1);
	//3 -optional reason

	//remove user form the channel: get user channel list and remove the chhanel by name
	std::map<std::string,User>::iterator it = user_list.find(user_name);
    if (it!=user_list.end())
	{
		std::map<std::string,Channel>::iterator it2 = channels_list.find(channel_name);
		if (it2!=channels_list.end())
		{
			channels_list[channel_name].remove_user(user_list[user_name]);
			res.channel = channel_name;
			res.all_user = ":KickerNick!user@host KICK #channel TargetNick :Reason";
		}
	}
	return res;
}