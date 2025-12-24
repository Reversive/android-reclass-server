#include <packet.hpp>
#include <cstring>

std::vector<char> network::packet::serialize() const
{
    std::vector<char> packet_bytes;
    packet_bytes.reserve(_packet_size);
    packet_bytes.insert(packet_bytes.end(), (char *)&_packet_size, (char *)&_packet_size + sizeof(_packet_size));
    packet_bytes.insert(packet_bytes.end(), (char *)&_packet_id, (char *)&_packet_id + sizeof(_packet_id));
    std::visit([&](auto &&arg)
               {
        std::vector<char> data_bytes = arg.serialize();
        packet_bytes.insert(packet_bytes.end(), data_bytes.begin(), data_bytes.end()); },
               _data);
    return packet_bytes;
}

network::packet::packet(network::packet_type packet_id, const packet_data &data)
    : _packet_id(packet_id), _data(data)
{
    _packet_size = sizeof(_packet_size) + sizeof(_packet_id) + std::visit([](auto &&arg)
                                                                          { return arg.get_size(); },
                                                                          _data);
}
