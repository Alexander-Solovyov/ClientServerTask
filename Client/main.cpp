#include "Client.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

void parseArg(int argc, char *argv[], bool &isUDP, char *&address, char *&port)
{
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            continue;
        }

        if (tolower(argv[i][1]) == 'a')
        {
            address = argv[++i];
        }
        else if (tolower(argv[i][1] == 'p'))
        {
            port = argv[++i]; 
        }
        else if (!strcmp(argv[i], "-tcp"))
        {
            isUDP = false;
        }
    }
}

int main(int argc, char *argv[])
{
    bool isUDP = true;
    char *address = nullptr;
    char *port = nullptr;
    parseArg(argc, argv, isUDP, address, port);

    Client client(isUDP);
    if (!client.connect(address, strtol(port, nullptr, 0)))
    {
        std::cout << "client connect failed!";
        return 0;
    }
    
    std::string buffer;
    while (true)
    {
        getline(std::cin, buffer);
        client.sendMessage(buffer);
        client.getMessage(buffer);
        std::cout << buffer;
    }

    return 0;
}
