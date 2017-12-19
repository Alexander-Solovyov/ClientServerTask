#include "ServerUDP.h"

#include <sys/socket.h>
#include <cstring>

ServerUDP::ServerUDP() :
    socket_(0)
{
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
}

ServerUDP::~ServerUDP()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
    }
}

bool ServerUDP::bind(unsigned long address, unsigned short port)
{
    destinationAddress_.sin_family = AF_INET;
    destinationAddress_.sin_port = htons(port);
    destinationAddress_.sin_addr.s_addr = address;

    return ::bind(socket_, 
        (sockaddr *)&destinationAddress_, 
        sizeof(destinationAddress_)) >= 0;
}

UDPData *ServerUDP::getMessage() const
{
    return new UDPData(socket_);
}



UDPData::UDPData(int socket) :
    socket_(socket),
    length_(0),
    buffer_(new char[bufferSize_ + 1])
{
    unsigned addrLen = sizeof(address_);
    length_ = recvfrom(socket_,
                       buffer_,
                       bufferSize_,
                       0,
                       (sockaddr *)&address_,
                       &addrLen);
}

UDPData::~UDPData()
{
    delete[] buffer_;
}


bool UDPData::getMessage(char *&message)
{
    if (length_ <= 0)
    {
        return false;
    }

    message = new char[length_ + 1];
    memcpy(message, buffer_, length_);
    message[length_] = '\0';
    return true;
}

bool UDPData::sendBack(const char *message)
{
    return sendto(socket_,
                  message,
                  strlen(message),
                  0,
                  (sockaddr *)&address_,
                  sizeof(address_)) >= 0;
}
