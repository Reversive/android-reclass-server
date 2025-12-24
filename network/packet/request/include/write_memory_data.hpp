#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

namespace request
{
    class write_memory_data
    {
    public:
        write_memory_data(int process_id, uint64_t address, std::vector<char> data)
            : _process_id(process_id), _address(address), _data(std::move(data)) {}

        int get_process_id() const { return _process_id; }
        uint64_t get_address() const { return _address; }
        const std::vector<char>& get_data() const { return _data; }
        int get_write_size() const { return static_cast<int>(_data.size()); }

        std::vector<char> serialize() const;
        static write_memory_data deserialize(const std::vector<char>& data);

        static constexpr size_t min_size()
        {
            return sizeof(int) + sizeof(uint64_t) + sizeof(int);
        }

        int get_size() const
        {
            return static_cast<int>(min_size() + _data.size());
        }

    private:
        int _process_id;
        uint64_t _address;
        std::vector<char> _data;
    };
}
