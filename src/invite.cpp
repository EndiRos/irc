#include <iostream>
#include "users.hpp"
#include "channel.hpp"

std::string invite(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
{
	(void)user;
    std::string res;
    size_t pos = msg.find(' ') + 1;
    size_t pos2 = msg.find(' ');
    std::string user_name = msg.substr(pos, pos2 - pos);
    std::string channel_name = msg.substr(msg.find('#') + 1, msg.size());
    std::map<std::string,User>::iterator it = user_list.find(user_name);
    std::map<std::string,Channel>::iterator it2=channels_list.find(channel_name);
    if (it!=user_list.end() && it2!=channels_list.end())
    {  
        user_list[user_name].add_invite(channel_name);

        res = ":server 341 " + user.get_nick() + " " + user_name + " #" + channel_name + "\r\n" ;
        //TODO
        //---------PARA MANDAR AL OTRO USER----
        //res2 = ":"+user.get_nick()+"!"+user.get_name()+"@"+user.get_ip()+" INVITE "+user_name+" :#"+channel_name+ "\r\n" ;
        //res2 =  :Alice!alice@host INVITE Bob :#secretchannel
    }
    return res;
}