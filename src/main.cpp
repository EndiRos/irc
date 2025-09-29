/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enetxeba <enetxeba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:00:49 by enetxeba          #+#    #+#             */
/*   Updated: 2025/09/29 09:07:33 by enetxeba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "network.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "args must be port and password in this order" << std::endl;
        return 1;
    }    
    std::string password =argv[2];
    if (!is_valid_port(argv[1])) {
        std::cerr << "Puerto inválido" << std::endl;
        return 1;
    }
    int port = atoi(argv[1]);
    Network *net = new Network(port, password);
    (void) net;
    return 0;
}