#include "include/socket.hpp"

bool network::tcp::socket::bind()
{
    this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket < 0)
    {
        logger::error("Failed to create the socket");
        return false;
    }

    int opt = 1;
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::memset(&this->_server_address, 0, sizeof(this->_server_address));
    this->_server_address.sin_family = AF_INET;
    this->_server_address.sin_addr.s_addr = INADDR_ANY;
    this->_server_address.sin_port = htons(this->_port);
    if (::bind(this->_socket, (struct sockaddr *)&this->_server_address, sizeof(this->_server_address)) < 0)
    {
        logger::error("Failed to bind the socket");
        return false;
    }

    return true;
}

bool network::tcp::socket::listen()
{
    if (::listen(this->_socket, BACKLOG) < 0)
    {
        logger::error("Failed to listen the socket");
        return false;
    }
    return true;
}

bool network::tcp::socket::accept()
{
    this->_client_descriptor = ::accept(this->_socket, (struct sockaddr *)&this->_client_address, &this->_client_address_length);
    if (this->_client_descriptor < 0)
    {
        logger::error("Failed to accept the client");
        return false;
    }
    return true;
}

bool network::tcp::socket::close()
{
    logger::info("Closing the connection...");
    close_client();
    if (this->_socket != -1)
    {
        if (::close(this->_socket) < 0)
        {
            logger::error("Failed to close the socket");
            return false;
        }
        this->_socket = -1;
    }
    logger::info("Connection closed successfully");
    return true;
}

void network::tcp::socket::close_client()
{
    if (this->_client_descriptor != -1)
    {
        ::close(this->_client_descriptor);
        this->_client_descriptor = -1;
    }
}

network::tcp::socket::socket(long port)
{
    this->_port = port;
    this->_socket = this->_client_descriptor = -1;
}

network::tcp::socket::~socket()
{
    this->close();
}

void *network::tcp::socket::receive(size_t size)
{
    int bytes_received = 0;
    void *buffer = malloc(size);
    if (buffer == nullptr)
    {
        logger::error("Failed to allocate memory for the buffer");
        return nullptr;
    }
    while (bytes_received < size)
    {
        int read_bytes = ::recv(this->_client_descriptor, static_cast<char *>(buffer) + bytes_received, size - bytes_received, 0);
        if (read_bytes < 0)
        {
            logger::error("Failed to receive data from the client");
            return nullptr;
        }
        else if (read_bytes == 0)
        {
            logger::info("Client disconnected");
            return nullptr;
        }
        bytes_received += read_bytes;
    }

    return buffer;
}

int network::tcp::socket::receive_int()
{
    int *buffer = static_cast<int *>(this->receive(sizeof(int)));
    if (buffer == nullptr)
    {
        return -1;
    }
    int value = *buffer;
    free(buffer);
    return value;
}

std::vector<char> network::tcp::socket::receive_byte_array(size_t size)
{
    std::vector<char> buffer(size);
    int bytes_received = 0;
    while (bytes_received < size)
    {
        int read_bytes = ::recv(this->_client_descriptor, &buffer[bytes_received], size - bytes_received, 0);
        if (read_bytes < 0)
        {
            logger::error("Failed to receive data from the client");
            return std::vector<char>();
        }
        else if (read_bytes == 0)
        {
            logger::info("Client disconnected");
            return std::vector<char>();
        }
        bytes_received += read_bytes;
    }

    return buffer;
}

bool network::tcp::socket::send_byte_array(const std::vector<char> &buffer)
{
    int bytes_sent = 0;
    while (bytes_sent < buffer.size())
    {
        int sent_bytes = ::send(this->_client_descriptor, &buffer[bytes_sent], buffer.size() - bytes_sent, 0);
        if (sent_bytes < 0)
        {
            logger::error("Failed to send data to the client");
            return false;
        }
        bytes_sent += sent_bytes;
    }
    return true;
}