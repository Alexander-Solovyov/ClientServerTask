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

void gatherData(const char *message)
{
    int numbers[10] = { };
    const char key[] = "0123456789\n";
    --message;
    while ((message = strpbrk(message + 1, key)) != nullptr)
    {
        if (!isdigit(*message))
        {
            break;
        }
        ++numbers[*message - '0'];
    }
    long sum = 0;
    int minimum = 10;
    int maximum = 0;
    for (int i = 9; i >= 0; --i)
    {
        if (numbers[i])
        {
            minimum = std::min(minimum, i);
            maximum = std::max(maximum, i);
            sum += numbers[i] * i;

            while (numbers[i] > 0)
            {
                std::cout << i << " ";
                --numbers[i];
            }
        }
    }
    if (minimum != 10)
    {
        std::cout << std::endl << "Sum: " << sum << std::endl
                  << "Min: " << minimum << std::endl
                  << "Max: " << maximum << std::endl;
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
            gatherData(message);
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
            gatherData(message);
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