#pragma once
#include <string>
#include "users.hpp"
#include "channel.hpp"
#include "msg.hpp"

msg_  kick(std::string &msg, User& user, std::map<std::string, Channel> &channels_list, std::map<std::string, User> &user_list);