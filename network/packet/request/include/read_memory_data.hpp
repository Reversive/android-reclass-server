#pragma once
#include <vector>
namespace request
{
    class read_memory_data
    {
    public:
        read_memory_data(int process_id, void *address, int size)
            : _process_id(process_id), _address(address), _size(size) {}
        int get_process_id() const { return _process_id; }
        void *get_address() const { return _address; }
        int get_read_size() const { return _size; }
        std::vector<char> serialize();
        static read_memory_data deserialize(const std::vector<char> &data);
        int get_size() const;

    private:
        int _process_id;
        void *_address;
        int _size;
    };
};