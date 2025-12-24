#pragma once
#include <vector>
namespace response
{
    class memory_data
    {
    public:
        memory_data() {}
        memory_data(const std::vector<char> &data) : _data(data) {}
        std::vector<char> get_data() const { return _data; }
        std::vector<char> serialize() const;
        int get_size() const;

    private:
        std::vector<char> _data;
    };
}