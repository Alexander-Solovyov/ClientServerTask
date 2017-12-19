#include <iostream>
#include <cstring>
#include <thread>

#include "ServerUDP.h"
#include "ServerTCP.h"

void parseArg(int argc, char *argv[], char *&portTCP, char *&portUDP)
{
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-tcp"))
        {
            portTCP = argv[++i];
        }
        else if (!strcmp(argv[i], "-udp"))
        {
            portUDP = argv[++i];
        }
    }
}

void processUDP(int port)
{
    ServerUDP serverUDP;
    if (!serverUDP.bind(0, port))
    {
        std::cout << "ServerUDP bind failed!";
        return;
    }

    char *message = nullptr;
    UDPData *data = nullptr;
    while (true)
    {
        data = serverUDP.getMessage();
        if (data->getMessage(message))
        {
            data->sendBack(message);
            delete[] message;
        }
        delete data;
    }
}

void processTCP(int port)
{
    ServerTCP serverTCP;
    if (!serverTCP.bind(0, port))
    {
        std::cout << "ServerTCP bind failed!";
        return;
    }

    char *message = nullptr;
    TCPConnection *connection = nullptr;
    while (true)
    {
        connection = serverTCP.connect();
        
        while (connection->getMessage(message))
        {
            connection->sendMessage(message);
            delete[] message;
        }
        delete connection;
    }
}

int main(int argc, char *argv[])
{
    char *portTCP = nullptr;
    char *portUDP = nullptr;
    parseArg(argc, argv, portTCP, portUDP);
    int tcp = (portTCP ? strtol(portTCP, nullptr, 0) : 5555);
    int udp = (portUDP ? strtol(portUDP, nullptr, 0) : 9999);

    std::thread udpThread(processUDP, udp);
	processTCP(tcp);
    udpThread.join();
	return 0; 
}