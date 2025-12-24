#include <server.hpp>

namespace
{
    constexpr int PACKET_HEADER_SIZE = sizeof(int) * 2;

    network::packet_type get_response_type(network::packet_type request_type)
    {
        switch (request_type)
        {
            case network::packet_type::packet_get_process_list_req:
                return network::packet_type::packet_get_process_list_res;
            case network::packet_type::packet_read_memory_req:
                return network::packet_type::packet_read_memory_res;
            case network::packet_type::packet_write_memory_req:
                return network::packet_type::packet_write_memory_res;
            default:
                return network::packet_type::packet_error;
        }
    }

    bool is_valid_request_type(network::packet_type type)
    {
        return type == network::packet_type::packet_get_process_list_req ||
               type == network::packet_type::packet_read_memory_req ||
               type == network::packet_type::packet_write_memory_req;
    }
}

network::server::server(int port)
    : _socket(std::make_unique<tcp::socket>(port))
{
}

bool network::server::run()
{
    logger::info("Starting TCP server on port %d...", _socket->get_port());

    if (!_socket->bind())
    {
        return false;
    }

    if (!_socket->listen())
    {
        return false;
    }

    logger::info("TCP server started on port %d", _socket->get_port());

    while (true)
    {
        logger::info("Waiting for a client...");

        if (!_socket->accept())
        {
            continue;
        }

        logger::info("Client connected");

        while (true)
        {
            auto incoming_packet = receive_packet();
            if (!incoming_packet)
            {
                break;
            }

            auto packet_id = incoming_packet->get_packet_id();
            if (!is_valid_request_type(packet_id))
            {
                logger::error("Invalid packet type: %d", static_cast<int>(packet_id));
                break;
            }

            int handler_index = static_cast<int>(packet_id);
            network::packet_data response_payload = network::packet_handlers[handler_index](incoming_packet->get_data());

            network::packet response_packet(get_response_type(packet_id), response_payload);

            if (!send_packet(response_packet))
            {
                break;
            }
        }

        logger::info("Client disconnected");
        _socket->close_client();
    }

    return true;
}

bool network::server::send_packet(const network::packet& packet)
{
    std::vector<char> data = const_cast<network::packet&>(packet).serialize();
    return _socket->send_byte_array(data);
}

std::unique_ptr<network::packet> network::server::receive_packet()
{
    int packet_size;
    auto result = _socket->receive_int(packet_size);
    if (result != tcp::recv_result::success)
    {
        if (result == tcp::recv_result::error)
            logger::error("Failed to receive packet size");
        return nullptr;
    }

    if (packet_size < PACKET_HEADER_SIZE)
    {
        logger::error("Invalid packet size: %d (too small)", packet_size);
        return nullptr;
    }

    if (static_cast<size_t>(packet_size) > tcp::MAX_PACKET_SIZE)
    {
        logger::error("Packet size exceeds maximum: %d", packet_size);
        return nullptr;
    }

    int packet_id_raw;
    result = _socket->receive_int(packet_id_raw);
    if (result != tcp::recv_result::success)
    {
        if (result == tcp::recv_result::error)
            logger::error("Failed to receive packet id");
        return nullptr;
    }

    auto packet_id = static_cast<network::packet_type>(packet_id_raw);

    size_t data_size = static_cast<size_t>(packet_size) - PACKET_HEADER_SIZE;
    if (data_size == 0)
    {
        return std::make_unique<network::packet>(packet_size, packet_id);
    }

    std::vector<char> packet_data;
    result = _socket->receive_byte_array(packet_data, data_size);
    if (result != tcp::recv_result::success)
    {
        if (result == tcp::recv_result::error)
            logger::error("Failed to receive packet data");
        return nullptr;
    }

    if (packet_id == network::packet_type::packet_read_memory_req)
    {
        if (packet_data.size() < request::read_memory_data::min_size())
        {
            logger::error("Truncated read_memory packet");
            return nullptr;
        }
        return std::make_unique<network::packet>(packet_size, packet_id, request::read_memory_data::deserialize(packet_data));
    }
    else if (packet_id == network::packet_type::packet_write_memory_req)
    {
        if (packet_data.size() < request::write_memory_data::min_size())
        {
            logger::error("Truncated write_memory packet");
            return nullptr;
        }
        return std::make_unique<network::packet>(packet_size, packet_id, request::write_memory_data::deserialize(packet_data));
    }

    return nullptr;
}
