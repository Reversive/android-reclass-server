#pragma once
#include <packet.hpp>
#include <logger.hpp>
#include <util.hpp>
#include <syscall.hpp>
#include <memory_data.hpp>

namespace network
{
    namespace handler
    {
        network::packet_data get_process_list(const packet_data &);
        network::packet_data read_memory(const packet_data &);
        network::packet_data write_memory(const packet_data &);
    }

    static network::packet_handler_fn packet_handlers[3] = {
        network::handler::get_process_list,
        network::handler::read_memory,
        network::handler::write_memory};
}