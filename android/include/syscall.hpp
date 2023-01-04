#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>

namespace android
{
    namespace syscall
    {
        extern int process_vm_readv_syscall;
        extern int process_vm_writev_syscall;

        ssize_t process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
                          const struct iovec *__remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite);

        bool pvm(pid_t __pid, void *address, void *buffer, size_t size, bool iswrite);

        bool vm_readv(pid_t __pid, void *address, void *buffer, size_t size);

        bool vm_writev(pid_t __pid, void *address, void *buffer, size_t size);
    }
}