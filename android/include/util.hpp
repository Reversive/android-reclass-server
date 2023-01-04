#pragma once
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <process_info.hpp>
#include <fcntl.h>

namespace android
{
    namespace util
    {
        std::vector<process_info> get_processes();
    };
};