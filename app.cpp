#include <app.hpp>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <atomic>

namespace
{
    std::atomic<bool> g_shutdown_requested{false};

    void signal_handler(int)
    {
        g_shutdown_requested.store(true, std::memory_order_relaxed);
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

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    network::server srv(static_cast<int>(port));
    return srv.run();
}
