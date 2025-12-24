#include <write_memory_data.hpp>

std::vector<char> request::write_memory_data::serialize()
{
    std::vector<char> data_bytes;
    int data_size = static_cast<int>(_data.size());
    data_bytes.reserve(sizeof(_process_id) + sizeof(_address) + sizeof(data_size) + data_size);
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&_process_id), reinterpret_cast<const char*>(&_process_id) + sizeof(_process_id));
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&_address), reinterpret_cast<const char*>(&_address) + sizeof(_address));
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&data_size), reinterpret_cast<const char*>(&data_size) + sizeof(data_size));
    data_bytes.insert(data_bytes.end(), _data.begin(), _data.end());
    return data_bytes;
}

request::write_memory_data request::write_memory_data::deserialize(const std::vector<char> &data)
{
    int process_id;
    uintptr_t address;
    int data_size;

    size_t offset = 0;
    std::memcpy(&process_id, data.data() + offset, sizeof(process_id));
    offset += sizeof(process_id);
    std::memcpy(&address, data.data() + offset, sizeof(address));
    offset += sizeof(address);
    std::memcpy(&data_size, data.data() + offset, sizeof(data_size));
    offset += sizeof(data_size);

    std::vector<char> write_data(data.begin() + offset, data.begin() + offset + data_size);
    return request::write_memory_data(process_id, address, write_data);
}

int request::write_memory_data::get_size() const
{
    return sizeof(_process_id) + sizeof(_address) + sizeof(int) + static_cast<int>(_data.size());
}
