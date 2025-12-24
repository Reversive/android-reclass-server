#pragma once
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdarg>

class logger
{
public:
    static void debug(const char *fmt, ...);
    static void info(const char *fmt, ...);
    static void error(const char *fmt, ...);
    [[noreturn]] static void fatal(const char *fmt, ...);
};