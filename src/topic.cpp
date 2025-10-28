#include <iostream>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"
#include "topic.hpp"

void topic(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
{
    (void)user_list;
    msg_ res;
    size_t pos = msg.find(' ');
    size_t pos2 = msg.find(':');
    std::string channel_name;
    if (pos2 != std::string::npos)
        channel_name = msg.substr(msg.find(' ') + 1, pos2 - pos -2);
    else
        channel_name = msg.substr(pos + 1, msg.find('\r') - pos -1);
    std::map<std::string,Channel>::iterator it=channels_list.find(channel_name);
    if (it != channels_list.end())
    {
      if(!channels_list[channel_name].topic_f)
       { 
        res.user = ":server 403 "+user.get_nick()+" "+channel_name+" :Can not change the topic"+ "\r\n";
        res.user += ":server 332 "+user.get_nick()+" "+channel_name+" :"+channels_list[channel_name].topic;

      }
      else if (pos2 == std::string::npos)
        res.user = ":server 332 "+user.get_nick()+" "+channel_name+" :"+channels_list[channel_name].topic;
      else
      {
        if (channels_list[channel_name].operators.find(user.get_nick()) == channels_list[channel_name].operators.end())
        {
            res.user = ":server 332 "+user.get_nick()+" "+channel_name+" :"+channels_list[channel_name].topic;    
            Commands::send_to_all(user.get_fd(),channels_list,res);
            Commands::send_to_one(user.get_fd(),res);
            return ;
        }
        channels_list[channel_name].topic = msg.substr(pos2 + 1, msg.size());
        res.user = ":"+user.get_nick()+"!"+user.get_name()+"@"+user.get_ip()+" TOPIC "+" "+channel_name +" :"+ channels_list[channel_name].topic;
        res.channel = channel_name;
      }
    }
	  else
      res.user = ":server 403 "+user.get_nick()+" "+channel_name+" :No such channel"+ "\r\n";
    res.all_user = res.user;
    res.channel = channel_name;
    Commands::send_to_all(user.get_fd(),channels_list,res);
    Commands::send_to_one(user.get_fd(),res);
    return;
}
