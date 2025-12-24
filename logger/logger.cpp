#include "logger.hpp"
#include <cstdio>
#include <stdexcept>

void log(std::ostream &out, const char *color, const char *level, const char *fmt, va_list args)
{
    std::time_t now = std::time(nullptr);
    std::tm tm_now;
    localtime_r(&now, &tm_now);

    out << "\033[" << color << "m[" << tm_now.tm_hour << ":" << tm_now.tm_min << ":" << tm_now.tm_sec << "] [" << level << "]\033[0m ";

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    out << buffer << std::endl;
}

void logger::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(std::cerr, "36", "DEBUG", fmt, args);
    va_end(args);
}

void logger::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(std::cerr, "32", "INFO", fmt, args);
    va_end(args);
}

void logger::error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(std::cerr, "31", "ERROR", fmt, args);
    va_end(args);
}

void logger::fatal(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(std::cerr, "31", "FATAL", fmt, args);
    va_end(args);

    char buffer[1024];
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    throw std::runtime_error(buffer);
}