#pragma once
#include <string>
#include <vector>
#include <cstring>

class process_info
{
public:
    process_info(int process_id, const std::string &process_name)
        : _process_id(process_id), _process_name(process_name) {}
    int get_process_id() const { return _process_id; }
    std::string get_process_name() const { return _process_name; }

    std::vector<char> serialize() const
    {
        std::vector<char> data;
        int name_len = static_cast<int>(_process_name.size());
        data.reserve(sizeof(_process_id) + sizeof(name_len) + name_len);
        data.insert(data.end(), reinterpret_cast<const char*>(&_process_id), reinterpret_cast<const char*>(&_process_id) + sizeof(_process_id));
        data.insert(data.end(), reinterpret_cast<const char*>(&name_len), reinterpret_cast<const char*>(&name_len) + sizeof(name_len));
        data.insert(data.end(), _process_name.begin(), _process_name.end());
        return data;
    }

    int get_serialized_size() const
    {
        return sizeof(_process_id) + sizeof(int) + static_cast<int>(_process_name.size());
    }

private:
    int _process_id;
    std::string _process_name;
};