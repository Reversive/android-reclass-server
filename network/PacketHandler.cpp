#include "include/PacketHandler.hpp"


bool handleGetProcessListPacket(PacketRequest *req) {
    return true;
}

bool handleGetProcessInfoPacket(PacketRequest *req) {
    return true;
}

PacketHandler packetHandlers[] = { 
    handleGetProcessListPacket, 
    handleGetProcessInfoPacket 
};

bool handleIncomingPacket(PacketRequest *req) {
    return packetHandlers[req->type](req);
}