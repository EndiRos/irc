/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:19:43 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/30 10:45:40 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quit.hpp" 

void quit(User &user,std::map<std::string,User> &user_list, std::map<std::string, Channel> &channel_list)
{
    msg_ ret;
    
    std::map<std::string,User>::iterator users = user_list.find(user.get_nick());
    if (users == user_list.end()){
        ret.user = ("Error: user not found");
        return;
    }
    ret.user =  ":server NOTICE *: " + user.get_nick() + " has gone.\n";
    std::map<std::string,User>::iterator userss = user_list.begin();
    std::map<std::string,User>::iterator users_end = user_list.end();
    for (;userss != users_end; ++userss)
    {
        int fd = userss->second.get_fd();
        if (fd != 0)
            Commands::send_to_one(fd, ret);
    }
    remove_user_all_channels(user,channel_list);
    return;
}

void remove_user_all_channels(User user, std::map<std::string, Channel> &channel_list)
{
     std::map<std::string, Channel>::iterator channels_it =  channel_list.begin();

     for (;channels_it != channel_list.end(); ++channels_it)
     {
        std::map<std::string, User>::iterator user_it = (*channels_it).second.users.find(user.get_nick());
        if (user_it != (*channels_it).second.users.end())
            (*channels_it).second.users.erase(user_it);
     }
}