#pragma once
#include <socket.hpp>
#include <handler.hpp>
namespace network
{

    class server
    {
    public:
        server(long port);
        ~server();
        network::packet *receive_packet();
        bool send_packet(network::packet *packet);
        bool run();

    private:
        tcp::socket *_socket;
    };
};