#pragma once
#include <string>
class process_info
{
public:
    process_info(int process_id, const std::string &process_name)
        : _process_id(process_id), _process_name(process_name) {}
    int get_process_id() const { return _process_id; }
    std::string get_process_name() const { return _process_name; }

private:
    int _process_id;
    std::string _process_name;
};