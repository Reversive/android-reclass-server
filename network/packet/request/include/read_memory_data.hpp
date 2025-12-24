#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

namespace request
{
    class read_memory_data
    {
    public:
        read_memory_data(int process_id, uintptr_t address, int size)
            : _process_id(process_id), _address(address), _size(size) {}
        int get_process_id() const { return _process_id; }
        uintptr_t get_address() const { return _address; }
        int get_read_size() const { return _size; }
        std::vector<char> serialize();
        static read_memory_data deserialize(const std::vector<char> &data);
        int get_size() const;

    private:
        int _process_id;
        uintptr_t _address;
        int _size;
    };
}