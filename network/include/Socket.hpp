#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

class Socket {
private:
    int mSockFd;
    int mPort;
    int mClientFd;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientLen;
    
public:
    Socket(long port);
    ~Socket();
    bool bind();
    bool listen();
    bool accept();
    char *recv(size_t size, size_t *readBytes);
    bool send(const char *data, size_t size);
    void close();
};


#endif