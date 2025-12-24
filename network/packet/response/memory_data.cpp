#include <memory_data.hpp>

std::vector<char> response::memory_data::serialize() const
{
    return _data;
}

int response::memory_data::get_size() const
{
    return _data.size();
}
