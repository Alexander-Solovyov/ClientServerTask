#pragma once

#include <netinet/in.h>

class TCPConnection
{
public:
    TCPConnection(int socket);
    ~TCPConnection();
    bool getMessage(char *&message);
    bool sendMessage(const char *message);

private:
    int socket_;
    const int bufferSize_ = 1024;
    char *buffer_;
};

class ServerTCP
{
public:
	ServerTCP();
	~ServerTCP();

    bool bind(unsigned long address = 0, unsigned short port = 9999);
    TCPConnection *connect() const;

private:
    int socket_;
    sockaddr_in destinationAddress_;
};
