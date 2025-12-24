#include <handler.hpp>
#include <stdexcept>

network::packet_data network::handler::get_process_list(const packet_data &data)
{
    std::vector<process_info> processes = android::util::get_processes();
    return response::process_list_data(processes.size(), processes);
}

network::packet_data network::handler::read_memory(const packet_data &data)
{
    const auto* read_data = std::get_if<request::read_memory_data>(&data);
    if (!read_data)
    {
        throw std::runtime_error("Invalid packet data for read_memory");
    }

    uint32_t read_size = read_data->get_read_size();
    if (read_size == 0 || read_size > request::read_memory_data::MAX_READ_SIZE)
    {
        logger::error("Invalid read size: %u", read_size);
        return response::memory_data();
    }

    std::vector<char> buffer(read_size);

    if (!android::syscall::vm_readv(read_data->get_process_id(), reinterpret_cast<void*>(read_data->get_address()), buffer.data(), buffer.size()))
    {
        logger::error("Failed to read memory at 0x%lx", read_data->get_address());
        return response::memory_data();
    }

    return response::memory_data(buffer);
}

network::packet_data network::handler::write_memory(const packet_data &data)
{
    const auto* write_data = std::get_if<request::write_memory_data>(&data);
    if (!write_data)
    {
        throw std::runtime_error("Invalid packet data for write_memory");
    }

    const std::vector<char>& buffer = write_data->get_data();

    if (!android::syscall::vm_writev(write_data->get_process_id(), reinterpret_cast<void*>(write_data->get_address()), const_cast<char*>(buffer.data()), buffer.size()))
    {
        logger::error("Failed to write memory at 0x%lx", write_data->get_address());
        return response::memory_data();
    }

    return response::memory_data(buffer);
}