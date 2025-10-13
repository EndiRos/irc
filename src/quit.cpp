/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:19:43 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/10 11:24:04 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quit.hpp" 

void quit(User &user,std::map<std::string,User> &user_list)
{
    msg_ ret;
    
    std::map<std::string,User>::iterator users = user_list.find(user.get_nick());
    if (users == user_list.end()){
        ret.user = ("Error: user not found");
        return;
    }
    ret.user = ":server QUIT" + user.get_nick() + " :Quit bye, ciao, agur benhur \n";
    Commands::send_to_one(user.get_fd(), ret);
    user_list[user.get_nick()].set_fd(0);
    ret.user =  ":server NOTICE *: " + user.get_nick() + " has gone.\n";
    std::map<std::string,User>::iterator userss = user_list.begin();
    std::map<std::string,User>::iterator users_end = user_list.end();
    for (;userss != users_end; ++userss)
    {
        int fd = userss->second.get_fd();
        if (fd != 0)
            Commands::send_to_one(fd, ret);
    }
    ret.channel="";

    return;
}