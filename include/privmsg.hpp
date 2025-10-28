/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:41:21 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/27 14:15:47 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "commands.hpp"

void privmsg(std::string trim , User user,  std::map<std::string, Channel> &channels_list,std::map<std::string, User> &users);