#include <util.hpp>

std::vector<process_info> android::util::get_processes()
{
    std::vector<process_info> processes;
    DIR *dir = opendir("/proc");
    if (dir == nullptr)
    {
        return processes;
    }
    dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        int process_id = -1;
        if (sscanf(entry->d_name, "%d", &process_id) != 1)
        {
            continue;
        }
        char cmdline_path[64];
        snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%d/cmdline", process_id);
        char process_name[64];
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
    closedir(dir);
    return processes;
}