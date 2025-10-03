/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imugica- <imugica-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:14:26 by enetxeba          #+#    #+#             */
/*   Updated: 2025/10/03 11:06:21 by imugica-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <stdlib.h>

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

bool is_valid_port(const char* str) {
    // Verifica que todos los caracteres sean dígitos
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    int port = atoi(str);
    // Verifica rango de puerto válido
    return port >= 1 && port <= 65535;
}

void clean_msg(std::string& ib)
{

    if (ib.substr(0,3)== "CAP")
    {
        for (int i = 0; i < 4; i++)
        {
            std::string::size_type pos = ib.find('\n');
            if (pos == std::string::npos)
                break;
            ib.erase(0,pos + 1);
        }
    }
   
}

