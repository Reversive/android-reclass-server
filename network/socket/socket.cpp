#include "include/socket.hpp"

network::tcp::socket::socket(int port)
    : _port(port)
{
}

network::tcp::socket::~socket()
{
    close();
}

bool network::tcp::socket::bind()
{
    _socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        logger::error("Failed to create socket: %s", strerror(errno));
        return false;
    }

    int opt = 1;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = INADDR_ANY;
    _server_address.sin_port = htons(_port);

    if (::bind(_socket, reinterpret_cast<struct sockaddr*>(&_server_address), sizeof(_server_address)) < 0)
    {
        logger::error("Failed to bind socket: %s", strerror(errno));
        return false;
    }

    return true;
}

bool network::tcp::socket::listen()
{
    if (::listen(_socket, BACKLOG) < 0)
    {
        logger::error("Failed to listen: %s", strerror(errno));
        return false;
    }
    return true;
}

bool network::tcp::socket::accept()
{
    _client_address_length = sizeof(_client_address);
    _client_descriptor = ::accept(_socket, reinterpret_cast<struct sockaddr*>(&_client_address), &_client_address_length);
    if (_client_descriptor < 0)
    {
        logger::error("Failed to accept: %s", strerror(errno));
        return false;
    }
    return true;
}

bool network::tcp::socket::close()
{
    close_client();
    if (_socket != -1)
    {
        if (::close(_socket) < 0)
        {
            logger::error("Failed to close socket: %s", strerror(errno));
            return false;
        }
        _socket = -1;
    }
    return true;
}

void network::tcp::socket::close_client()
{
    if (_client_descriptor != -1)
    {
        ::close(_client_descriptor);
        _client_descriptor = -1;
    }
}

network::tcp::recv_result network::tcp::socket::receive_bytes(void* buffer, size_t size)
{
    size_t bytes_received = 0;
    char* buf = static_cast<char*>(buffer);

    while (bytes_received < size)
    {
        ssize_t n = ::recv(_client_descriptor, buf + bytes_received, size - bytes_received, 0);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            logger::error("recv failed: %s", strerror(errno));
            return recv_result::error;
        }
        if (n == 0)
        {
            return recv_result::disconnected;
        }
        bytes_received += static_cast<size_t>(n);
    }
    return recv_result::success;
}

network::tcp::recv_result network::tcp::socket::receive_int(int& out_value)
{
    return receive_bytes(&out_value, sizeof(out_value));
}

network::tcp::recv_result network::tcp::socket::receive_byte_array(std::vector<char>& out_buffer, size_t size)
{
    out_buffer.resize(size);
    recv_result result = receive_bytes(out_buffer.data(), size);
    if (result != recv_result::success)
    {
        out_buffer.clear();
    }
    return result;
}

bool network::tcp::socket::send_byte_array(const std::vector<char>& buffer)
{
    size_t bytes_sent = 0;

    while (bytes_sent < buffer.size())
    {
        ssize_t n = ::send(_client_descriptor, buffer.data() + bytes_sent, buffer.size() - bytes_sent, 0);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            logger::error("send failed: %s", strerror(errno));
            return false;
        }
        bytes_sent += static_cast<size_t>(n);
    }
    return true;
}
