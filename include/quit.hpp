/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:20:59 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/09 14:16:42 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "commands.hpp"

void quit( User &user,std::map<std::string,User> &users);