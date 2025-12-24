#include <read_memory_data.hpp>
#include <stdexcept>

std::vector<char> request::read_memory_data::serialize() const
{
    std::vector<char> data_bytes(min_size());
    char* ptr = data_bytes.data();

    std::memcpy(ptr, &_process_id, sizeof(_process_id));
    ptr += sizeof(_process_id);
    std::memcpy(ptr, &_address, sizeof(_address));
    ptr += sizeof(_address);
    std::memcpy(ptr, &_size, sizeof(_size));

    return data_bytes;
}

request::read_memory_data request::read_memory_data::deserialize(const std::vector<char>& data)
{
    int process_id;
    uint64_t address;
    uint32_t size;

    const char* ptr = data.data();
    std::memcpy(&process_id, ptr, sizeof(process_id));
    ptr += sizeof(process_id);
    std::memcpy(&address, ptr, sizeof(address));
    ptr += sizeof(address);
    std::memcpy(&size, ptr, sizeof(size));

    if (size > MAX_READ_SIZE)
    {
        throw std::runtime_error("Read size exceeds maximum allowed");
    }

    return read_memory_data(process_id, address, size);
}