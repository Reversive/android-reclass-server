#include <app.hpp>

network::server *server = nullptr;

void signal_handler(int signal)
{
    if (server != nullptr)
    {
        delete server;
    }
    exit(0);
}

bool app::run(int argc, char *argv[])
{
    logger::info("Remember to 'adb forward tcp:<port> tcp:<port>' before running this program.");
    signal(SIGINT, signal_handler);
    if (argc != 2)
    {
        logger::error("Usage: %s <port>", argv[0]);
        return false;
    }

    long port = strtol(argv[1], NULL, 10);
    if (port < 0l)
    {
        logger::error("Invalid port: %d", port);
        return false;
    }
    server = new network::server(port);
    return server->run();
}
