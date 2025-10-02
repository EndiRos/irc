#include <iostream>
#include "channel.hpp"
#include <cstdlib>


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

std::string execute_i(bool mode, Channel &my_channel)
{
	std::cout << "execute i" << std::endl;
	my_channel.invite_f = mode;
	return mode ? "Now the channel need invitation\n" : "Now channel is open\n";
}

std::string execute_t(bool mode, Channel &my_channel)
{
	std::cout << "execute t" << std::endl;
	my_channel.topic_f = mode;
	return mode ? "Now the topic is editable\n" : "Now the topic is not editable\n";
}

std::string execute_k(std::string &msg, bool mode, Channel &my_channel)
{
	if (!mode)
	{
		my_channel.key_f = mode;
		return "Now the pass is not needed\n";
	}
	else
	{
		std::string result = find_param_k(msg);
		if (result == "")
			return "there is not in a database, u need to defin one";
		else
		{
			my_channel.key_f = mode;
			my_channel.key = result;
			return "Now the channel is protected by password";
		}
	}
}

std::string  execute_o(std::string &msg, bool mode, Channel &my_channel)
{
	std::string user = find_param_o(msg);
	if (user != "" && mode)
	{
		std::map<std::string, User>::iterator it = my_channel.users.find(user);
		if (it != my_channel.users.end())
			my_channel.operators.insert(*it);
		return "Now" + user + " is op";
	}
	if (user != "" && !mode)
	{
		std::map<std::string, User>::iterator it = my_channel.operators.find(user);
		if (it != my_channel.operators.end())
			my_channel.operators.erase(user);
		return "Now" + user + " is no longer an op";
	}
	return "Error: you must insert user name";
}

std::string execute_l(std::string &msg, bool mode, Channel &my_channel)
{
    (void)my_channel;
	std::string result;
	if (mode)
	{
        result = find_param_l(msg);
        my_channel.user_limit = atoi(result.c_str());
        my_channel.limit_f = true;
        return "Now the limit of user is " + result;
    }
    else{
        my_channel.limit_f = false;
        return "There is not limit of user";
    }

	if (result != "")
		std::cout << result << std::endl;
	return "Error: you must give a param";
}