/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:31:41 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/28 11:31:45 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"
#include "commands.hpp"

void topic(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list);
