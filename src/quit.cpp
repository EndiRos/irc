/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:19:43 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/07 10:44:28 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quit.hpp" 

msg_ quit(User user,std::map<std::string,User> user_list)
{
    msg_ ret;
    user.set_fd(0);
    std::map<std::string,User>::iterator users = user_list.find(user.get_name());
    if (users == user_list.end()){
        ret.user = ("Error: user not found");
        return (ret);
    }
    user_list[user.get_nick()].set_fd(0);
    ret.user = ":" + user.get_nick() + "!" + user.get_name()+"@" + user.get_ip() + " QUIT :";
    ret.user += "bye bye, ciao, agur benhur";
    ret.all_user = ret.user = ":" + user.get_nick() + "!" + user.get_name()+"@" + user.get_ip() + " QUIT :";
    ret.all_user +=  " has gone.";
    ret.channel="";
    //std::cout << ret.all_user<<  std::endl;
    
    return ret;
}