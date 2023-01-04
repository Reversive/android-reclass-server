#include <process_list_data.hpp>

std::vector<char> response::process_list_data::serialize()
{
    std::vector<char> data_bytes;
    data_bytes.reserve(sizeof(_process_count) + _process_list.size() * sizeof(process_info));
    data_bytes.insert(data_bytes.end(), (char *)&_process_count, (char *)&_process_count + sizeof(_process_count));
    for (auto &process : _process_list)
    {
        data_bytes.insert(data_bytes.end(), (char *)&process, (char *)&process + sizeof(process_info));
    }
    return data_bytes;
}

int response::process_list_data::get_size() const
{
    return sizeof(_process_count) + _process_list.size() * sizeof(process_info);
}