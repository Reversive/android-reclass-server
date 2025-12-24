#include <read_memory_data.hpp>

std::vector<char> request::read_memory_data::serialize()
{
    std::vector<char> data_bytes;
    data_bytes.reserve(sizeof(_process_id) + sizeof(_address) + sizeof(_size));
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&_process_id), reinterpret_cast<const char*>(&_process_id) + sizeof(_process_id));
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&_address), reinterpret_cast<const char*>(&_address) + sizeof(_address));
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&_size), reinterpret_cast<const char*>(&_size) + sizeof(_size));
    return data_bytes;
}

request::read_memory_data request::read_memory_data::deserialize(const std::vector<char> &data)
{
    int process_id;
    uintptr_t address;
    int size;

    size_t offset = 0;
    std::memcpy(&process_id, data.data() + offset, sizeof(process_id));
    offset += sizeof(process_id);
    std::memcpy(&address, data.data() + offset, sizeof(address));
    offset += sizeof(address);
    std::memcpy(&size, data.data() + offset, sizeof(size));

    return request::read_memory_data(process_id, address, size);
}

int request::read_memory_data::get_size() const
{
    return sizeof(_process_id) + sizeof(_address) + sizeof(_size);
}