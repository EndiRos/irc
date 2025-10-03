#include <iostream>
#include "users.hpp"
#include "channel.hpp"

std::string topic(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
{
	(void)user;
	(void)user_list;
    //check if the channel exists
    //cheeck if the inivite command has a paramater, if not just print topic, else change topic if the flag is correct
    std::string res;
    size_t pos = msg.find('#');
    size_t pos2 = msg.find(' ', pos);
    std::string channel_name;
    if (pos2 != std::string::npos)
        channel_name = msg.substr(msg.find('#') + 1, pos2);
    else
        channel_name = msg.substr(msg.find('#') + 1, msg.size());

    std::map<std::string,Channel>::iterator it=channels_list.find(channel_name);
    if (it != channels_list.end())
    {
		if(channels_list[channel_name].topic_f)
        {
			res += "No se puede cambiar el topic!";
        }
		else if (pos2 != std::string::npos)
		{
			res += channels_list[channel_name].topic;
		}
		else
		{
			channels_list[channel_name].topic = msg.substr(pos2 + 1, msg.size());
			res += "Topic cambiado";
		}
    }
    return res;
}