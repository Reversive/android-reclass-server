#include <syscall.hpp>

#if defined(__arm__)
int android::syscall::process_vm_readv_syscall = 376;
int android::syscall::process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int android::syscall::process_vm_readv_syscall = 270;
int android::syscall::process_vm_writev_syscall = 271;
#elif defined(__i386__)
int android::syscall::process_vm_readv_syscall = 347;
int android::syscall::process_vm_writev_syscall = 348;
#else
int android::syscall::process_vm_readv_syscall = 310;
int android::syscall::process_vm_writev_syscall = 311;
#endif

ssize_t android::syscall::process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
                                    const struct iovec *__remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite)
{
    return ::syscall((iswrite ? android::syscall::process_vm_writev_syscall : android::syscall::process_vm_readv_syscall), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

bool android::syscall::pvm(pid_t __pid, void *address, void *buffer, size_t size, bool iswrite)
{
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (__pid < 0)
    {
        return false;
    }

    ssize_t bytes = process_v(__pid, local, 1, remote, 1, 0, iswrite);
    return bytes >= 0 && static_cast<size_t>(bytes) == size;
}

bool android::syscall::vm_readv(pid_t __pid, void *address, void *buffer, size_t size)
{
    return pvm(__pid, address, buffer, size, false);
}

bool android::syscall::vm_writev(pid_t __pid, void *address, void *buffer, size_t size)
{
    return pvm(__pid, address, buffer, size, true);
}