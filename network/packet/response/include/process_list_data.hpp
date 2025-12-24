#pragma once
#include <vector>
#include <process_info.hpp>
namespace response
{
    class process_list_data
    {
    public:
        process_list_data(int process_count, const std::vector<process_info> &_process_list)
            : _process_count(process_count), _process_list(_process_list) {}
        int get_process_count() const { return _process_count; }
        std::vector<process_info> get_processes() const { return _process_list; }
        std::vector<char> serialize() const;
        size_t get_size() const;

    private:
        int _process_count;
        std::vector<process_info> _process_list;
    };
}