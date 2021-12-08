#include "include/PacketHandler.hpp"

const char *getProcessNameByPid(const int pid) {
	char *name = (char *)calloc(MAX_DATA_SIZE, sizeof(char));
	if (name) {
		sprintf(name, "/proc/%d/cmdline", pid);
		FILE *f = fopen(name, "r");
		if (f) {
			size_t size;
			size = fread(name, sizeof(char), MAX_DATA_SIZE, f);
			if (size > 0) {
				if ('\n' == name[size - 1])
					name[size - 1] = '\0';
			}
			fclose(f);
		}
	}
	return name;
}

int getProcesses(ProcessInfo *processes) {
    DIR *proc = opendir("/proc");
	struct dirent *ent;
	long tgid;
    int count = 0;

	if (proc == NULL) {
		return count;
	}

    while ((ent = readdir(proc)) != NULL) {

        if(count == MAX_PROCESS_COUNT) break;

        if (!isdigit(*ent->d_name))
            continue;

        tgid = strtol(ent->d_name, NULL, 10);
        const char *processName = getProcessNameByPid(tgid);
        if (strstr(processName, "cmdline") != NULL || strstr(processName, "/") != NULL) {
            continue;
        }
        ProcessInfo process;
        strcpy(process.name, processName);
        process.pid = tgid;
        processes[count++] = process;
    }

    closedir(proc);
    return count;
}



bool handleGetProcessListPacket(Socket *sock, PacketRequest *req) {
    log(INFO, "%s", "Received GetProcessList packet");
    ProcessInfo processes[MAX_PROCESS_COUNT];
    int count = getProcesses(processes);
    if (count == -1) {
        log(ERROR, "%s", "Failed to get process list");
        return false;
    }
    PacketResponse response;
    response.type = PACKET_GET_PROCESS_LIST;
    response.status = OK;
    response.data.processListData.processCount = count;
    for (int i = 0; i < count; i++) {
        strcpy(response.data.processListData.processList[i].name, processes[i].name);
        response.data.processListData.processList[i].pid = processes[i].pid;
    }
    return sock->send((char*)&response, sizeof(response));
}

bool handleReadMemoryPacket(Socket *sock, PacketRequest *req) {
    target_pid = req->data.memoryDataRequest.processId;
    PacketResponse response;
    vm_readv(reinterpret_cast<void *>(req->data.memoryDataRequest.address), &response.data.memoryData.data, req->data.memoryDataRequest.size);
    response.type = PACKET_READ_MEMORY;
    response.status = OK;
    return sock->send((char*)&response, sizeof(response));
}

bool handleWriteMemoryPacket(Socket *sock, PacketRequest *req) {
    log(INFO, "%s", "Received WriteMemory packet");
    return true;
}

PacketHandler packetHandlers[] = { 
    handleGetProcessListPacket, 
    handleReadMemoryPacket, 
    handleWriteMemoryPacket
};

bool handleIncomingPacket(Socket *sock, PacketRequest *req) {
    return packetHandlers[req->type](sock, req);
}