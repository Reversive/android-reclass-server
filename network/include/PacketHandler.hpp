#ifndef PACKET_HANDLER_HPP
#define PACKET_HANDLER_HPP
#define MAX_PROCESS_COUNT 100
#define MAX_NAME_SIZE 128
#define MAX_DATA_SIZE 1024

typedef enum { PACKET_GET_PROCESS_LIST, PACKET_READ_MEMORY, PACKET_WRITE_MEMORY } PacketType;
typedef enum { READ, WRITE } AccessType;
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
    AccessType accessType;
    int processId;
    int address;
    int size;
} MemoryDataRequest;

typedef struct MemoryDataResponse {
    char data[MAX_DATA_SIZE];
} MemoryDataResponse;

typedef struct PacketRequest {
    PacketType type;
    typedef union {
        MemoryDataRequest memoryDataRequest;
    } Data;
} PacketRequest;

typedef bool (*PacketHandler)(PacketRequest *req);

typedef struct PacketResponse {
    PacketType type;
    Status status;
    typedef union {
        ProcessListDataResponse processListData;
        MemoryDataResponse memoryData;
    } Data;
} PacketResponse;

#endif