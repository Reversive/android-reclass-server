#include <read_memory_data.hpp>

std::vector<char> request::read_memory_data::serialize()
{
    std::vector<char> data_bytes;
    data_bytes.reserve(sizeof(_process_id) + sizeof(_address) + sizeof(_size));
    data_bytes.insert(data_bytes.end(), (char *)&_process_id, (char *)&_process_id + sizeof(_process_id));
    data_bytes.insert(data_bytes.end(), (char *)&_address, (char *)&_address + sizeof(_address));
    data_bytes.insert(data_bytes.end(), (char *)&_size, (char *)&_size + sizeof(_size));
    return data_bytes;
}

request::read_memory_data request::read_memory_data::deserialize(const std::vector<char> &data)
{
    int process_id, size;
    void *address;
    std::memcpy(&process_id, &data[0], sizeof(int));
    std::memcpy(&address, &data[sizeof(int)], sizeof(int));
    std::memcpy(&size, &data[2 * sizeof(int)], sizeof(int));
    return request::read_memory_data(process_id, address, size);
}

int request::read_memory_data::get_size() const
{
    return sizeof(_process_id) + sizeof(_address) + sizeof(_size);
}