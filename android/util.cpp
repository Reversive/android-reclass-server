#include <util.hpp>
#include <cstdio>
#include <cstring>
#include <memory>

namespace
{
    constexpr size_t MAX_PATH_LEN = 64;
    constexpr size_t MAX_PROCESS_NAME_LEN = 64;

    struct DirDeleter
    {
        void operator()(DIR* dir) const noexcept
        {
            if (dir) closedir(dir);
        }
    };
    using DirPtr = std::unique_ptr<DIR, DirDeleter>;
}

std::vector<process_info> android::util::get_processes()
{
    std::vector<process_info> processes;
    DirPtr dir(opendir("/proc"));
    if (!dir)
    {
        return processes;
    }
    dirent *entry;
    while ((entry = readdir(dir.get())) != nullptr)
    {
        int process_id = -1;
        if (sscanf(entry->d_name, "%d", &process_id) != 1)
        {
            continue;
        }
        char cmdline_path[MAX_PATH_LEN];
        snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%d/cmdline", process_id);
        char process_name[MAX_PROCESS_NAME_LEN];
        int fd = open(cmdline_path, O_RDONLY | O_CLOEXEC);
        if (fd < 0)
        {
            continue;
        }
        ssize_t len = read(fd, process_name, sizeof(process_name) - 1);
        close(fd);
        if (len <= 0)
        {
            continue;
        }
        process_name[len] = '\0';
        if (strstr(process_name, "cmdline") != nullptr || strchr(process_name, '/') != nullptr)
        {
            continue;
        }
        processes.emplace_back(process_info(process_id, process_name));
    }
    return processes;
}