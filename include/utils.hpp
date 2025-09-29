/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:12:57 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/29 09:09:14 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

std::string trim_msg(std::string line, int lenght,char cha = '\n', int init = 0);
void my_memset(void* ptr, int value, size_t num);
bool is_valid_port(const char* str);