#pragma once

#include <netinet/in.h>
#include <string>

using std::string;

class Client
{
public:
    Client(bool isUDP);
    virtual ~Client();

    virtual bool connect(const char* address = "127.0.0.1", unsigned short port = 9999);
    virtual bool getMessage(string &message);
    virtual bool sendMessage(const string &message);

protected:
    int socket_;
    sockaddr_in destinationAddress_;
    const int bufferSize_ = 1024;
    char *buffer_;
};
