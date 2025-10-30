/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 09:06:32 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/30 09:06:40 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"
#include "commands.hpp"

void  kick(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list);