/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 09:01:55 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/30 09:02:05 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"
#include "commands.hpp"

void invite(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list);
