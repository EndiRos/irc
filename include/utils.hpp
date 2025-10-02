/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:12:57 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/02 12:41:36 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <channel.hpp>

std::string trim_msg(std::string line, int lenght,char cha = '\n', int init = 0);
void my_memset(void* ptr, int value, size_t num);
bool is_valid_port(const char* str);
void clean_msg(std::string& ib);

std::string join(std::string msg,User user, std::map<std::string,Channel>& channel);

std::string find_param_k(std::string &msg);

std::string find_param_o(std::string &msg);

std::string find_param_l(std::string& msg);

std::string execute_i(bool mode, Channel &my_channel);

std::string execute_t(bool mode, Channel &my_channel);

std::string execute_k(std::string &msg, bool mode, Channel &my_channel);

std::string execute_o(std::string &msg, bool mode, Channel &my_channel);

std::string execute_l(std::string &msg, bool mode, Channel &my_channel);

std::string extract_channel(std::string msg);