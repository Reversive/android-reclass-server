#include <handler.hpp>

network::packet_data network::handler::get_process_list(const packet_data &data)
{
    std::vector<process_info> processes = android::util::get_processes();
    return response::process_list_data(processes.size(), processes);
}

network::packet_data network::handler::read_memory(const packet_data &data)
{
    request::read_memory_data read_data = std::get<request::read_memory_data>(data);
    std::vector<char> buffer(read_data.get_read_size());

    if (!android::syscall::vm_readv(read_data.get_process_id(), reinterpret_cast<void*>(read_data.get_address()), buffer.data(), buffer.size()))
    {
        logger::error("Failed to read memory at 0x%lx", read_data.get_address());
        return response::memory_data();
    }

    return response::memory_data(buffer);
}

network::packet_data network::handler::write_memory(const packet_data &data)
{
    request::write_memory_data write_data = std::get<request::write_memory_data>(data);
    const std::vector<char>& buffer = write_data.get_data();

    if (!android::syscall::vm_writev(write_data.get_process_id(), reinterpret_cast<void*>(write_data.get_address()), const_cast<char*>(buffer.data()), buffer.size()))
    {
        logger::error("Failed to write memory at 0x%lx", write_data.get_address());
        return response::memory_data();
    }

    return response::memory_data(buffer);
}