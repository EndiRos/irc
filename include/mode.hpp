/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:03:06 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/13 12:17:42 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"
#include <cstdlib>
#include "commands.hpp"

msg_ mode(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, msg_ &res);
std::string find_param_k(std::string &msg);
std::string find_param_o(std::string &msg);
std::string find_param_l(std::string& msg);
std::string execute_i(bool mode, Channel &my_channel);
std::string execute_t(bool mode, Channel &my_channel);
std::string execute_k(std::string &msg, bool mode, Channel &my_channel);
std::string execute_o(std::string &msg, bool mode, Channel &my_channel);
std::string execute_l(std::string &msg, bool mode, Channel &my_channel);
std::string extract_channel(std::string msg);