#pragma once

#include <netinet/in.h>

class UDPData
{
public:
    UDPData(int socket);
    ~UDPData();
    bool getMessage(char *&message);
    bool sendBack(const char *message);

private:
    int socket_;
    int length_;
    sockaddr_in address_;
    const int bufferSize_ = 1024;
    char *buffer_;
};

class ServerUDP
{
public:
    ServerUDP();
    ~ServerUDP();

    bool bind(unsigned long address = 0, unsigned short port = 9999);
    UDPData *getMessage() const;

private:
    int socket_;
    sockaddr_in destinationAddress_;
};
