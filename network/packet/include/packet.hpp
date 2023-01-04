#pragma once
#include <memory>
#include <vector>
#include <variant>
#include <process_list_data.hpp>
#include <memory_data.hpp>
#include <read_memory_data.hpp>
#include <no_data.hpp>

namespace network
{

    enum class packet_type
    {
        packet_error = -1,
        packet_get_process_list_req,
        packet_read_memory_req,
        packet_write_memory_req,
        packet_get_process_list_res,
        packet_read_memory_res,
        packet_write_memory_res
    };

    using packet_data = std::variant<response::process_list_data, response::memory_data, request::read_memory_data, request::no_data>;
    typedef packet_data (*packet_handler_fn)(const packet_data &);

    class packet
    {
    public:
        packet(int packet_size, network::packet_type packet_id, const packet_data &data)
            : _packet_size(packet_size), _packet_id(packet_id), _data(data) {}

        packet(int packet_size, network::packet_type packet_id)
            : packet(packet_size, packet_id, request::no_data{}) {}

        packet(network::packet_type packet_id, const packet_data &data);

        std::vector<char> serialize();
        int get_packet_size() const { return _packet_size; }
        network::packet_type get_packet_id() const { return _packet_id; }
        packet_data get_data() const { return _data; }

    private:
        int _packet_size;
        network::packet_type _packet_id;
        packet_data _data;
    };
}