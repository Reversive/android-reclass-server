#include <app.hpp>
#include <cstdlib>
#include <climits>

namespace
{
    volatile sig_atomic_t g_shutdown_requested = 0;

    void signal_handler(int)
    {
        g_shutdown_requested = 1;
    }
}

bool app::run(int argc, char* argv[])
{
    logger::info("Remember to 'adb forward tcp:<port> tcp:<port>' before running this program.");

    if (argc != 2)
    {
        logger::error("Usage: %s <port>", argv[0]);
        return false;
    }

    char* endptr = nullptr;
    long port = std::strtol(argv[1], &endptr, 10);

    if (endptr == argv[1] || *endptr != '\0' || port <= 0 || port > 65535)
    {
        logger::error("Invalid port: %s (must be 1-65535)", argv[1]);
        return false;
    }

    signal(SIGINT, signal_handler);

    network::server srv(static_cast<int>(port));
    return srv.run();
}
