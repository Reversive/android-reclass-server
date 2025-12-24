#pragma once
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <arpa/inet.h>
#include <string.h>
#include <cstring>
#include <logger.hpp>

#define BACKLOG 1

namespace network
{
    namespace tcp
    {
        class socket
        {
        public:
            socket(long port);
            ~socket();
            bool bind();
            bool listen();
            bool accept();
            bool close();
            void close_client();
            void *receive(size_t size);
            int receive_int();
            std::vector<char> receive_byte_array(size_t size);
            bool send_byte_array(const std::vector<char> &data);
            int get_port() const { return this->_port; };

        private:
            int _socket;
            int _port;
            int _client_descriptor;
            struct sockaddr_in _server_address;
            struct sockaddr_in _client_address;
            socklen_t _client_address_length;
        };
    }
}