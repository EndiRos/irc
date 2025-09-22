/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 09:45:28 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/19 12:11:49 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include <users.hpp>

std::map<std::string, data> User::users; 

User::User()
{
    user_.authen = false;
}
void User::set_nick(std::string nick) {
    user_.nick = nick
}
void User::set_authen(bool act) {
    user_.authen = act;
};
void User::set_name(std::string name){
    user_.name = name;
}
void User::set_ip(std::string ip){
    user_.ip = ip;
}
void User::set_port(uint16_t port){
    user_.port=port;
}
void User::add_channel(std::string chanel){
    //falta verificar que no existe dentro de la lista
    user_.chanels.emplace_back(chanel);
}
void User::add_user(){
    std::map<std::string, data>::value_type kv(user_.name, user_);
    User::users.insert(kv);
}
User::~User(){};
    
