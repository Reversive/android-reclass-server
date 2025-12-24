#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

namespace request
{
    class read_memory_data
    {
    public:
        static constexpr uint32_t MAX_READ_SIZE = 1024 * 1024;

        read_memory_data(int process_id, uint64_t address, uint32_t size)
            : _process_id(process_id), _address(address), _size(size) {}

        int get_process_id() const { return _process_id; }
        uint64_t get_address() const { return _address; }
        uint32_t get_read_size() const { return _size; }

        std::vector<char> serialize() const;
        static read_memory_data deserialize(const std::vector<char>& data);

        static constexpr size_t min_size()
        {
            return sizeof(int) + sizeof(uint64_t) + sizeof(uint32_t);
        }

        size_t get_size() const { return min_size(); }

    private:
        int _process_id;
        uint64_t _address;
        uint32_t _size;
    };
}