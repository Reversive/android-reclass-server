#ifndef PACKET_HANDLER_HPP
#define PACKET_HANDLER_HPP
#include "Socket.hpp"
#include <dirent.h>
#include <ctype.h>
#include "Process.hpp"
#define MAX_PROCESS_COUNT 100
#define MAX_NAME_SIZE 128
#define MAX_DATA_SIZE 1024


typedef enum { PACKET_GET_PROCESS_LIST, PACKET_READ_MEMORY, PACKET_WRITE_MEMORY } PacketType;
typedef enum { ERR = -1, OK = 0 } Status;
typedef struct ProcessInfo {
    int pid;
    char name[MAX_NAME_SIZE];
} ProcessInfo;

typedef struct ProcessListDataResponse {
    int processCount;
    ProcessInfo processList[MAX_PROCESS_COUNT];
} ProcessListDataResponse;

typedef struct MemoryDataRequest {
    int processId;
    uint64_t address;
    int size;
} MemoryDataRequest;

typedef struct MemoryDataResponse {
    char data[MAX_DATA_SIZE];
} MemoryDataResponse;

typedef struct PacketRequest {
    PacketType type;
    union {
        MemoryDataRequest memoryDataRequest;
    } data;
} PacketRequest;

typedef bool (*PacketHandler)(Socket *sock, PacketRequest *req);

typedef struct PacketResponse {
    PacketType type;
    Status status;
    union {
        ProcessListDataResponse processListData;
        MemoryDataResponse memoryData;
    } data;
} PacketResponse;

bool handleIncomingPacket(Socket *sock, PacketRequest *req);
#endif