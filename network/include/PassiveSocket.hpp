#ifndef PASSIVE_SOCKET_HPP
#define PASSIVE_SOCKET_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

class PassiveSocket {
private:
    int mSockFd;
    int mPort;
    int mClientFd;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientLen;
public:
    PassiveSocket(long port);
    ~PassiveSocket();
    bool bind();
    bool listen();
    bool accept();
    void close();
};


#endif