/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:09:09 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/24 10:30:02 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"
#include "users.hpp"
#include "utils.hpp"

void Commands::commands(std::string &line, User user)
{
    int pos = 0;
    int pos2 = 0; 
    pos2 = line.find(' ');
    int i = 0;
    for (; i < commnad_len_; i++ )
    {
        if (line.substr(pos,pos2) == comands_[i])
            break;
    }
    switch (i)
    {
    case 0:
        break;
    case 1:
        //nick(line, user);
        break;
    default:
        break;
    }
}

bool auth_user(std::string &msg, User &tmp_user_, std::string pass)
{   
    int pos = 0;
    while ((pos = msg.find('\n')) != std::string::npos) 
    {
        std::string line = msg.substr(0, pos);
        if (!line.empty() && line[line.size()-1] == '\r')
            line.erase(line.size()-1);
        msg.erase(0, pos + 1);
        if (line.substr(0,3) == "CAP")
            continue;// Soporta formato PASS <pwd>
        else if (line.substr(0,4) == "PASS")
        {
            std::string msg_header = trim_msg(line,4);
            if (msg_header != pass)
                return (1);
            tmp_user_.set_authen(true);
        }
        else if (line.substr(0,4) == "NICK")
            tmp_user_.set_nick( trim_msg(line,4));
        else if (line.substr(0,4) == "USER")
       {
            tmp_user_.set_name(trim_msg(line, 4, ' '));
            line.erase(0,line.find(':'));
            tmp_user_.set_real_name(trim_msg(line,line.find('\n')));
       }
    }
    return (0);
}