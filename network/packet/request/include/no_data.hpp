#pragma once
#include <vector>
namespace request
{
    class no_data
    {
    public:
        no_data() = default;
        std::vector<char> serialize() const
        {
            return std::vector<char>();
        }
        size_t get_size() const
        {
            return 0;
        }
    };
}