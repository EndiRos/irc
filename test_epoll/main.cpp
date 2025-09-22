
#include "network.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "args must be port and password in this order" << std::endl;
        return 1;
    }    
    std::string password =argv[2];
    uint16_t port = atoi (argv[1]);
    Network *net = new Network(port, password);
    return 0;
}