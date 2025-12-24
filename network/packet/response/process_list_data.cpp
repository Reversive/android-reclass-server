#include <process_list_data.hpp>

std::vector<char> response::process_list_data::serialize() const
{
    std::vector<char> data_bytes;
    data_bytes.reserve(get_size());
    data_bytes.insert(data_bytes.end(), reinterpret_cast<const char*>(&_process_count), reinterpret_cast<const char*>(&_process_count) + sizeof(_process_count));
    for (const auto &process : _process_list)
    {
        std::vector<char> process_data = process.serialize();
        data_bytes.insert(data_bytes.end(), process_data.begin(), process_data.end());
    }
    return data_bytes;
}

int response::process_list_data::get_size() const
{
    int size = sizeof(_process_count);
    for (const auto &process : _process_list)
    {
        size += process.get_serialized_size();
    }
    return size;
}