#include <iostream>
#include "channel.hpp"
#include <cstdlib>

std::string find_param_k(std::string &msg)
{
	std::cout << "finding k command param: " << std::endl;
    size_t pos = msg.find(' ');
    if (pos == std::string::npos)
    {
        std::cout << "No parameters found." << std::endl;
        return "";
    }
    pos++;
    while (pos < msg.size())
    {
        size_t end = msg.find(' ', pos);
        std::string param = (end == std::string::npos) ? msg.substr(pos) : msg.substr(pos, end - pos);
        std::cout << "Trying parameter: [" << param << "]" << std::endl;
		if (end == std::string::npos)
            msg.erase(pos);
        else
            msg.erase(pos, (end - (pos - 1)));
        return param;
    }
    std::cout << "No valid parameter found." << std::endl;
    return "";
}

std::string find_param_o(std::string &msg)
{
	std::cout << "finding o command param: " << std::endl;
    size_t pos = msg.find(' ');
    if (pos == std::string::npos)
    {
        std::cout << "No parameters found." << std::endl;
        return "";
    }
    pos++;
    while (pos < msg.size())
    {
        size_t end = msg.find(' ', pos);
        std::string param = (end == std::string::npos) ? msg.substr(pos) : msg.substr(pos, end - pos);
        std::cout << "Trying parameter: [" << param << "]" << std::endl;
		if (end == std::string::npos)
            msg.erase(pos);
        else
            msg.erase(pos, (end - (pos - 1)));
        return param;
    }
    std::cout << "No valid parameter found." << std::endl;
    return "";
}

std::string find_param_l(std::string& msg)
{
    std::cout << "finding L command param: " << std::endl;
    size_t pos = msg.find(' ');
    if (pos == std::string::npos)
    {
        std::cout << "No parameters found." << std::endl;
        return "";
    }
    pos++;
    while (pos < msg.size())
    {
        size_t end = msg.find(' ', pos);
        std::string param = (end == std::string::npos) ? msg.substr(pos) : msg.substr(pos, end - pos);
        std::cout << "Trying parameter: [" << param << "]" << std::endl;
        int num = atoi(param.c_str());
        if (num != 0 || param == "0")
        {
            std::cout << "Valid number found: " << num << std::endl;
            if (end == std::string::npos)
                msg.erase(pos);
            else
                msg.erase(pos, (end - (pos - 1)));
            return param;
        }
        else
            std::cout << "Invalid number format for: " << param << ", trying next..." << std::endl;
        if (end == std::string::npos)
            break;
        pos = end + 1;
    }
    std::cout << "No valid numeric parameter found." << std::endl;
    return "";
}

bool execute_i(bool mode, Channel &my_channel)
{
	std::cout << "execute i" << std::endl;
	my_channel.invite_f = mode;
	return true;
}

bool execute_t(bool mode, Channel &my_channel)
{
	std::cout << "execute t" << std::endl;
	my_channel.topic_f = mode;
	return true;
}

bool execute_k(std::string &msg, bool mode, Channel &my_channel)
{
	if (!mode)
	{
		my_channel.key_f = mode;
		return true;
	}
	else
	{
		std::string result = find_param_k(msg);
		if (result == "")
			return false;
		else
		{
			my_channel.key_f = mode;
			my_channel.key = result;
			return true;
		}
	}
}

bool execute_o(std::string &msg, bool mode, Channel &my_channel)
{
	std::string user = find_param_o(msg);
	if (user != "" && mode)
	{
		std::map<std::string, User>::iterator it = my_channel.users.find(user);
		if (it != my_channel.users.end())
			my_channel.operators.insert(*it);
		return true;
	}
	if (user != "" && !mode)
	{
		std::map<std::string, User>::iterator it = my_channel.operators.find(user);
		if (it != my_channel.operators.end())
			my_channel.operators.erase(user);
		return true;
	}
	return false;
}

bool execute_l(std::string &msg, bool mode, Channel &my_channel)
{
    (void)my_channel;
	std::string result;
	if (mode)
		result = find_param_l(msg);
	if (result != "")
		std::cout << result << std::endl;
	return true;
}