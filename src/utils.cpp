/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imugica- <imugica-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:14:26 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/02 14:01:42 by imugica-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <stdlib.h>

std::string trim_msg(std::string line, int lenght, char cha, int init)
{
    line.erase(init,lenght);
    while (!line.empty() && (line[0]==' '||line[0]=='\t'))
        line.erase(0,1);
    int to = line.find(cha);
    return  (line.substr(0,to));
}
void my_memset(void* ptr, int value, size_t num) {
    unsigned char* p = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < num; ++i)
        p[i] = static_cast<unsigned char>(value);
}

bool is_valid_port(const char* str) {
    // Verifica que todos los caracteres sean dígitos
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    int port = atoi(str);
    // Verifica rango de puerto válido
    return port >= 1 && port <= 65535;
}

void clean_msg(std::string& ib)
{

    if (ib.substr(0,3)== "CAP")
    {
        for (int i = 0; i < 4; i++)
        {
            std::string::size_type pos = ib.find('\n');
            if (pos == std::string::npos)
                break;
            ib.erase(0,pos + 1);
        }
    }
   
}
std::string join(std::string msg, User user, std::map<std::string,Channel> &channels)
{
    std::string::size_type pos =  msg.find(' ') + 1;
    std::string::size_type pos2 = msg.find('\r',pos);
    std::string re_channel = msg.substr(pos,pos2 - pos);
    std::map<std::string,Channel>::iterator it=channels.find(re_channel);
    std::string res;
    if (it==channels.end())
    {
        Channel tmp_chan(user,re_channel);
        tmp_chan.add_operators(user);
        channels[re_channel]=tmp_chan;
        res = ":"+ user.get_nick() + "@" + user.get_ip() + " JOIN :" + re_channel + "\r\n" ;
        res += ":server 353 "+ user.get_nick() + " = #" + re_channel +" :" + user.get_nick() + "\r\n" ;
        res += ":server 366 "+ user.get_nick() + " #" + re_channel +" :End of /NAMES list.\r\n" ;
    }
    return res;
}

std::string invite(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list)
{
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
