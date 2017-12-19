#include "Client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

Client::Client(bool isUDP) :
    socket_(0),
    buffer_(new char[bufferSize_ + 1])
{
    if (isUDP)
    {
        socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    } 
    else
    {
        socket_ = socket(AF_INET, SOCK_STREAM, 0);
    }
}

Client::~Client()
{
    delete [] buffer_;
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
    }
}

bool Client::connect(const char* address, unsigned short port)
{
    destinationAddress_.sin_family = AF_INET;
    destinationAddress_.sin_port = htons(port);

    if (inet_aton(address, &destinationAddress_.sin_addr) <= 0)
    {
        return false;
    }

    return ::connect(socket_, 
        (sockaddr *)&destinationAddress_, 
        sizeof(destinationAddress_)) >= 0;
}

bool Client::sendMessage(const string &message)
{
    int numberOfPackets = 1 + message.length() / bufferSize_;
    bool success = true;

    int currentLength = bufferSize_;
    const char *strStart = message.data();
    for (int i = 0; i < numberOfPackets; ++i)
    {
        if (i + 1 == numberOfPackets)
        {
            currentLength = 1 + message.length() % bufferSize_;
            strcpy(buffer_, strStart);
            buffer_[currentLength - 1] = '\n';
            buffer_[currentLength] = '\0';
            strStart = buffer_;
        }
        success = send(socket_,
                       strStart,
                       currentLength,
                       0) >= 0 && success;
        strStart += bufferSize_;
    }

    return success;
}

bool Client::getMessage(string &message)
{
    message.clear();
    while (recv(socket_,
                buffer_,
                bufferSize_,
                0) > 0)
    {
        if (char *pos = strchr(buffer_, '\n'))
        {
            message.append(buffer_, pos - buffer_ + 1);
            break;
        }
        message.append(buffer_);
    }

    return true;
}
