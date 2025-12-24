#pragma once
#include <socket.hpp>
#include <handler.hpp>
#include <memory>

namespace network
{
    class server
    {
    public:
        explicit server(int port);

        server(const server&) = delete;
        server& operator=(const server&) = delete;

        bool run();

    private:
        std::unique_ptr<network::packet> receive_packet();
        bool send_packet(const network::packet& packet);

        std::unique_ptr<tcp::socket> _socket;
    };
}