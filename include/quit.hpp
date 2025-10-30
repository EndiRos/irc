/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:20:59 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/30 11:03:58 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "commands.hpp"

void quit( User &user,std::map<std::string,User> &users,std::map<std::string, Channel> &channel_list);
void remove_user_all_channels(User user, std::map<std::string, Channel> &channel_list);