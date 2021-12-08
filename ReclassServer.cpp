#include "ReclassServer.hpp"

Socket *server;

void sigIntHandler(int signo) {
    log(INFO, "%s", "Received stop signal, stopping server....");
    if(server != nullptr) {
        delete server;
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    int status = 0;
    if(argc != 2) {
        log(ERROR, "Usage: %s <port>", argv[0]);
        return 1;
    }
    long port = strtol(argv[1], NULL, 10);
    log(INFO, "%s", "Remember to 'adb forward tcp:<port> tcp:<port>' before running this program.");
    log(INFO, "Starting server on port %ld...", port);
    signal(SIGINT, sigIntHandler);
    server = new Socket(port);
    if(server == nullptr) {
        log(ERROR, "%s", "Could not create server socket.");
        status = -1;
        goto cleanup;
    }

    if(server->bind() == false) {
        log(ERROR, "%s", "Bind failed");
        status = -2;
        goto cleanup;
    }

    if(server->listen() == false) {
        log(ERROR, "%s", "Listen failed");
        status = -3;
        goto cleanup;
    }

    log(INFO, "TCP server started on port %ld", port);

    if(server->accept() == false) {
        log(ERROR, "%s", "Accept failed");
        status = -4;
        goto cleanup;
    }

    log(INFO, "%s", "Client connected");

    while(true) {
        size_t bytesRead = 0;
        char *buffer = server->recv(sizeof(PacketRequest), &bytesRead);
        if(bytesRead == 0) break;
        if(buffer == nullptr) {
            log(ERROR, "%s", "Could not read from client");
            status = -5;
            goto cleanup;
        }
        
        PacketRequest *request = (PacketRequest *) buffer;
        if(!handleIncomingPacket(server, request)) {
            log(ERROR, "%s", "Could not handle incoming packet");
            status = -6;
            goto cleanup;
        }
    }


cleanup:
    if(server != nullptr) {
        delete server;
    }
    return status;
}