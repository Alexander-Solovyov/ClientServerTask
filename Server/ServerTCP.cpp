#include "ServerTCP.h"

#include <sys/socket.h>
#include <cstring>
#include <cstdlib>

ServerTCP::ServerTCP()
{
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
}

ServerTCP::~ServerTCP()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
    }
}

bool ServerTCP::bind(unsigned long address, unsigned short port)
{
    destinationAddress_.sin_family = AF_INET;
    destinationAddress_.sin_port = htons(port);
    destinationAddress_.sin_addr.s_addr = address;

    if (::bind(socket_, 
        (sockaddr *)&destinationAddress_, 
        sizeof(destinationAddress_)) < 0)
    {
        return false;
    }

    return (listen(socket_, 10) >= 0);
}

TCPConnection *ServerTCP::connect() const
{
    return new TCPConnection(accept(socket_, nullptr, nullptr));
}



TCPConnection::TCPConnection(int socket) :
    socket_(socket),
    buffer_(new char[bufferSize_ + 1])
{
}

TCPConnection::~TCPConnection()
{
    delete[] buffer_;
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
    }
}

bool TCPConnection::getMessage(char *&message)
{
    int length =  recv(socket_,
                       buffer_,
                       bufferSize_,
                       0);

    if (length <= 0)
    {
        return false;
    }

    message = new char[length + 1];
    memcpy(message, buffer_, length);
    message[length] = '\0';
    return true;
}

bool TCPConnection::sendMessage(const char *message)
{
    return send(socket_,
                message,
                strlen(message),
                0) >= 0;
}
