/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:01:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/03 12:37:56 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>
#include "users.hpp"
#include "channel.hpp"
#include "mode.hpp"
#include "join.hpp"
#include "utils.hpp"
#include <unistd.h>
#include "msg.hpp"
#include "quit.hpp"


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
    void send_to_all(int fd,std::map<std::string,Channel> chanel_list, std::map<std::string, User> user, msg_  msg);
};

std::string extract_channel(std::string msg);


