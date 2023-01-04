#pragma once
#include <vector>
namespace request
{
    class no_data
    {
    public:
        no_data() = default;
        std::vector<char> serialize()
        {
            return std::vector<char>();
        }
        int get_size() const
        {
            return 0;
        }
    };
}