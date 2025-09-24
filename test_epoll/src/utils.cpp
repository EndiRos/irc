/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:14:26 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/24 10:35:49 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string trim_msg(std::string line, int lenght, char cha, int init)
{
    line.erase(0,lenght);
    while (!line.empty() && (line[0]==' '||line[0]=='\t'))
        line.erase(0,1);
    int to = line.find(cha);
    return  (line.substr(0,to));
}