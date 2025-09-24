/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:01:56 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/24 12:55:28 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"

bool auth_user(std::string &msg, User &tmp_user_, std::string pass);
class Commands: public User
{
private:
    std::string comands_[24];
    int commnad_len_ ;

public:
    Commands();
    void commands(std::string &msg, User user);
    ~Commands();
};

