#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

namespace request
{
    class write_memory_data
    {
    public:
        write_memory_data(int process_id, uintptr_t address, const std::vector<char> &data)
            : _process_id(process_id), _address(address), _data(data) {}
        int get_process_id() const { return _process_id; }
        uintptr_t get_address() const { return _address; }
        const std::vector<char>& get_data() const { return _data; }
        int get_write_size() const { return static_cast<int>(_data.size()); }
        std::vector<char> serialize();
        static write_memory_data deserialize(const std::vector<char> &data);
        int get_size() const;

    private:
        int _process_id;
        uintptr_t _address;
        std::vector<char> _data;
    };
}
