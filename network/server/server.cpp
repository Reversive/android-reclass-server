#include <server.hpp>

network::server::server(long port)
{
    this->_socket = new tcp::socket(port);
}

network::server::~server()
{
    if (this->_socket != nullptr)
    {
        delete this->_socket;
    }
}

network::packet_type get_response_type(network::packet_type request_type)
{
    int response_type = static_cast<int>(request_type) + 3;
    return static_cast<network::packet_type>(response_type);
}

bool network::server::run()
{

    logger::info("Starting TCP server on port %ld...", this->_socket->get_port());
    if (!this->_socket->bind())
    {
        logger::error("Failed to bind the socket");
        return false;
    }

    if (!this->_socket->listen())
    {
        logger::error("Failed to listen to the socket");
        return false;
    }

    logger::info("TCP server started on port %ld, waiting for a client...", this->_socket->get_port());

    if (!this->_socket->accept())
    {
        logger::error("Failed to accept the client");
        return false;
    }

    logger::info("Client connected");
    bool error_ocurred = false;
    while (!error_ocurred)
    {
        network::packet *incoming_packet = this->receive_packet();
        if (incoming_packet == nullptr)
        {
            logger::error("Failed to receive the packet");
            error_ocurred = true;
            break;
        }

        network::packet_data response_payload = network::packet_handlers[static_cast<int>(incoming_packet->get_packet_id())](incoming_packet->get_data());

        network::packet *response_packet = new network::packet(get_response_type(incoming_packet->get_packet_id()), response_payload);
        if (!this->send_packet(response_packet))
        {
            error_ocurred = true;
            delete incoming_packet;
            delete response_packet;
            break;
        }

        delete incoming_packet;
        delete response_packet;
    }
    return error_ocurred;
}

bool network::server::send_packet(network::packet *packet)
{
    std::vector<char> packet_data = packet->serialize();
    if (!this->_socket->send_byte_array(packet_data))
    {
        logger::error("Failed to send the response packet");
        return false;
    }
    return true;
}

network::packet *network::server::receive_packet()
{
    int packet_size = this->_socket->receive_int();
    if (packet_size == -1)
    {
        logger::error("Failed to receive the packet size");
        return nullptr;
    }
    network::packet_type packet_id = static_cast<network::packet_type>(this->_socket->receive_int());
    if (packet_id == network::packet_type::packet_error)
    {
        logger::error("Failed to receive the packet id");
        return nullptr;
    }
    size_t packet_data_size = packet_size - sizeof(int) * 2;
    if (packet_data_size == 0)
    {
        return new network::packet(packet_size, packet_id);
    }
    std::vector<char> packet_data = this->_socket->receive_byte_array(packet_data_size);
    if (packet_data.size() == 0)
    {
        logger::error("Failed to receive the packet data");
        return nullptr;
    }
    if (packet_id == network::packet_type::packet_read_memory_req)
    {
        return new network::packet(packet_size, packet_id, request::read_memory_data::deserialize(packet_data));
    }
    else if (packet_id == network::packet_type::packet_write_memory_req)
    {
        return new network::packet(packet_size, packet_id, request::write_memory_data::deserialize(packet_data));
    }
    return nullptr;
}
