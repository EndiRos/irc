/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:01:26 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/09 12:25:51 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "users.hpp"
#include "commands.hpp"

msg_ who(std::string msg, User &user, std::map<std::string,Channel> &channels);