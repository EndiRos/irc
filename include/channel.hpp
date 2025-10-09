/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:34:04 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/07 12:44:06 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "users.hpp"
#include <map>
#include <iostream>

class Channel
{
	public:
		std::string c_name;
		std::string topic;
		bool invite_f;
		bool topic_f;
		bool key_f;
		bool limit_f;
		std::string key;
		int user_limit;
	
		Channel();
		Channel(User &creator, std::string &name);
		~Channel();
		void add_user(User& new_user);
		void add_operators(User& new_ope);
		void remove_user(User& old_user);
		void remove_operator(User& old_ope);
		std::map<std::string, User> users;
		std::map<std::string, User> operators;
		std::string user_list();
};

std::string extract_channel(std::string msg);
