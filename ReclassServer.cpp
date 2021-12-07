#include "ReclassServer.hpp"

PassiveSocket *server;


void sigIntHandler(int signo) {
    log(INFO, "%s", "Received stop signal, stopping server....");
    if(server != NULL) {
        delete server;
    }
    exit(0);
}


int main(int argc, char *argv[]) {
    int status = 0;
    long port = strtol(argv[1], NULL, 10);
    log(INFO, "%s", "Remember to 'adb forward tcp:<port> tcp:<port>' before running this program.");
    log(INFO, "Starting server on port %ld...", port);
    signal(SIGINT, sigIntHandler);
    server = new PassiveSocket(port);
    if(server == NULL) {
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

    // Just accepting 1 connection, the plugin one.
    if(server->accept() == false) {
        log(ERROR, "%s", "Accept failed");
        status = -4;
        goto cleanup;
    }

    log(INFO, "%s", "Client connected");



cleanup:
    if(server != NULL) {
        delete server;
    }
    return status;
}