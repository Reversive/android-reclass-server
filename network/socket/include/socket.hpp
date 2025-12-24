#pragma once
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <logger.hpp>

constexpr int BACKLOG = 5;
constexpr size_t MAX_PACKET_SIZE = 16 * 1024 * 1024;

namespace network
{
    namespace tcp
    {
        enum class recv_result
        {
            success,
            error,
            disconnected
        };

        class socket
        {
        public:
            explicit socket(int port);
            ~socket();

            socket(const socket&) = delete;
            socket& operator=(const socket&) = delete;
            socket(socket&& other) noexcept;
            socket& operator=(socket&& other) noexcept;

            bool bind();
            bool listen();
            bool accept();
            bool close();
            void close_client();

            recv_result receive_bytes(void* buffer, size_t size);
            recv_result receive_int(int& out_value);
            recv_result receive_byte_array(std::vector<char>& out_buffer, size_t size);
            bool send_byte_array(const std::vector<char>& data);

            int get_port() const { return _port; }

        private:
            int _socket = -1;
            int _port;
            int _client_descriptor = -1;
            struct sockaddr_in _server_address{};
            struct sockaddr_in _client_address{};
            socklen_t _client_address_length = sizeof(sockaddr_in);
        };
    }
}