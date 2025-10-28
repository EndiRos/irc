/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:01:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/28 11:43:16 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>
#include "users.hpp"
#include "channel.hpp"
#include "mode.hpp"
#include "join.hpp"
#include "kick.h"
#include "utils.hpp"
#include <unistd.h>
#include "msg.hpp"
#include "quit.hpp"
#include "topic.hpp"
#include "who.hpp"
#include "invite.h"
#include "privmsg.hpp"


class Commands: public User
{
private:
    std::string comands_name_[24];
    int commnad_len_ ;
  
public:
    Commands();
    ~Commands();
    void execute(std::string &msg, User& user, std::map<std::string, User> &user_list, std::map<std::string, Channel> &channels_list);
    bool authorize(std::string &msg, User &tmp_user_, std::string pass, std::map<std::string, User> &user_list);
    void add_user(User &user, std::map<std::string, User> &user_list);
    static void send_to_one(int fd, msg_ res);
    static  void send_to_all(int fd,std::map<std::string,Channel> chanel_list, msg_  msg);
    static void refresh_users(User &user,std::map<std::string,Channel> &channels, std::string &channel);
};

std::string extract_channel(std::string msg);


