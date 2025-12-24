#include <write_memory_data.hpp>

std::vector<char> request::write_memory_data::serialize() const
{
    std::vector<char> data_bytes(get_size());
    char* ptr = data_bytes.data();
    int data_size = static_cast<int>(_data.size());

    std::memcpy(ptr, &_process_id, sizeof(_process_id));
    ptr += sizeof(_process_id);
    std::memcpy(ptr, &_address, sizeof(_address));
    ptr += sizeof(_address);
    std::memcpy(ptr, &data_size, sizeof(data_size));
    ptr += sizeof(data_size);
    std::memcpy(ptr, _data.data(), _data.size());

    return data_bytes;
}

request::write_memory_data request::write_memory_data::deserialize(const std::vector<char>& data)
{
    int process_id;
    uint64_t address;
    int data_size;

    const char* ptr = data.data();
    std::memcpy(&process_id, ptr, sizeof(process_id));
    ptr += sizeof(process_id);
    std::memcpy(&address, ptr, sizeof(address));
    ptr += sizeof(address);
    std::memcpy(&data_size, ptr, sizeof(data_size));
    ptr += sizeof(data_size);

    if (data_size < 0 || static_cast<size_t>(data_size) > data.size() - min_size())
    {
        return write_memory_data(process_id, address, {});
    }

    std::vector<char> write_data(ptr, ptr + data_size);
    return write_memory_data(process_id, address, std::move(write_data));
}
