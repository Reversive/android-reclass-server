#include <handler.hpp>

network::packet_data network::handler::get_process_list(const packet_data &data)
{
    std::vector<process_info> processes = android::util::get_processes();
    return response::process_list_data(processes.size(), processes);
}

network::packet_data network::handler::read_memory(const packet_data &data)
{
    request::read_memory_data read_memory_data = std::get<request::read_memory_data>(data);
    logger::debug("Read memory request: pid: %d, address: %p, size: %d", read_memory_data.get_process_id(), read_memory_data.get_address(), read_memory_data.get_read_size());
    std::vector<char>
        buffer(read_memory_data.get_read_size());

    if (!android::syscall::vm_readv(read_memory_data.get_process_id(), read_memory_data.get_address(), buffer.data(), buffer.size()))
    {
        logger::error("Failed to read memory");
        return response::memory_data();
    }

    return response::memory_data(buffer);
}

network::packet_data network::handler::write_memory(const packet_data &data)
{
    // TODO
    return response::memory_data();
}