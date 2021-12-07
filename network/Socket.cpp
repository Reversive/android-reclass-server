#include "include/Socket.hpp"

LOG_LEVEL current_level = DEBUG;

Socket::Socket(long port) {
    this->mPort = port;
    this->mClientFd = this->mSockFd = -1;
}

bool Socket::bind() {
    this->mSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->mSockFd < 0) {
        log(ERROR, "%s", "Error opening socket");
        return false;
    }
    bzero((char *) &this->mServerAddr, sizeof(this->mServerAddr));
    this->mServerAddr.sin_family = AF_INET;
    this->mServerAddr.sin_addr.s_addr = INADDR_ANY;
    this->mServerAddr.sin_port = htons(this->mPort);
    if (::bind(this->mSockFd, (struct sockaddr *) &this->mServerAddr, sizeof(this->mServerAddr)) < 0) {
        perror("ERROR on binding");
        return false;
    }
    return true;
}

bool Socket::listen() {
    if (::listen(this->mSockFd, 5) < 0) {
        log(ERROR, "%s", "Error on listening");
        return false;
    }
    return true;
}

bool Socket::accept() {
    this->mClientFd = ::accept(this->mSockFd, (struct sockaddr *) &this->mClientAddr, &this->mClientLen);
    if (this->mClientFd < 0) {
        log(ERROR, "%s", "Error on accept");
        return false;
    }
    return true;
}


void Socket::close() {
    log(INFO, "%s", "Closing the connection...");
    if (this->mClientFd != -1) {
        ::close(this->mClientFd);
    }
    if (this->mSockFd != -1) {
        ::close(this->mSockFd);
    }
}

char *Socket::recv(size_t size, size_t *bytes_read) {
    char *buffer = new char[size];
    int n = ::recv(this->mClientFd, buffer, size, 0);
    if (n < 0) {
        log(ERROR, "%s", "Error reading from socket");
        return nullptr;
    }
    *bytes_read = n;
    return buffer;
}

bool Socket::send(const char *buffer, size_t size) {
    int n = ::send(this->mClientFd, buffer, size, 0);
    if (n < 0) {
        log(ERROR, "%s", "Error writing to socket");
        return false;
    }
    return true;
}

Socket::~Socket() {
    close();
}