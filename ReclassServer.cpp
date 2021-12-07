#include "ReclassServer.hpp"

PassiveSocket *server;


void sigHandler(int signo) {
    if (signo == SIGINT) {
        log(INFO, "%s", "Received stop signal, stopping server....");
        server->close();
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    long port = strtol(argv[1], NULL, 10);
    log(INFO, "%s", "Remember to 'adb forward tcp:<port> tcp:<port>' before running this program.");
    log(INFO, "Starting server on port %ld...", port);
    signal(SIGINT, sigHandler);
    server = new PassiveSocket(port);

    if(server->bind() == false) {
        log(ERROR, "%s", "Bind failed");
        exit(1);
    }

    if(server->listen() == false) {
        log(ERROR, "%s", "Listen failed");
        exit(1);
    }

    log(INFO, "TCP server started on port %ld", port);

    if(server->accept() == false) {
        log(ERROR, "%s", "Accept failed");
        exit(1);
    }

    log(INFO, "%s", "Client connected");
    
    server->close();
    return 0;
}