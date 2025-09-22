/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:01:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/19 12:36:29 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include "users.hpp"

class Commands: public User
{
private:
    std::string comands_[23] = {
        "PASS",
        "NICK",
        "USER",
        "QUIT",
       "PING",
        "CAP",
        "PRIVMSG",
       "NOTICE",
        "JOIN",
        "PART",
        "TOPIC",
        "NAMES",
        "LIST",
        "MODE",
        "INVITE",
        "KICK",
        "WHOIS",
        "WHO",
        "ISON",
        "USERHOST",
        "AWAY",
        "OPER",
        "KILL",
    };
    int commnad_len_ = comands_->length();

public:
    void commands(std::string &chain, User user);
    ~Commands();
};


