#include "channel.hpp"

Channel::Channel()
{
}

Channel::Channel(User &creator, std::string &name)
{
	this->c_name = name;
	this->users[creator.get_nick()] = creator;
	this->operators[creator.get_nick()] = creator;
	invite_f = false;
	topic_f = false;
	key_f = false;
	limit_f = false;
	key = "";
	user_limit = 0;
}

Channel::~Channel()
{
}

void Channel::add_user(User &new_user)
{
	this->users[new_user.get_nick()] = new_user;
}

void Channel::add_operators(User &new_ope)
{
	this->operators[new_ope.get_nick()] = new_ope;
}

void Channel::remove_user(User &new_user)
{
	users.erase(new_user.get_nick());
}

void Channel::remove_operator(User &old_ope)
{
	operators.erase(old_ope.get_nick());
}

std::string extract_channel(std::string msg)
{
    std::string channel;
    size_t pos = msg.find('#');
	pos++;
	while (msg[pos] != ' ' && msg[pos] != '+' && msg[pos] != '-' && msg[pos])
	{
		channel += (char)msg[pos];
		pos++;
	}
    return channel;
}
