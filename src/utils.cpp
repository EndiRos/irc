/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:14:26 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/24 13:15:49 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string trim_msg(std::string line, int lenght, char cha, int init)
{
    line.erase(init,lenght);
    while (!line.empty() && (line[0]==' '||line[0]=='\t'))
        line.erase(0,1);
    int to = line.find(cha);
    return  (line.substr(0,to));
}
void my_memset(void* ptr, int value, size_t num) {
    unsigned char* p = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < num; ++i)
        p[i] = static_cast<unsigned char>(value);
}