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
};