/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commnad.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 09:09:09 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/19 13:02:13 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "users.hpp"

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
        nick(line, user);
        break;
    default:
        break;
    }
}