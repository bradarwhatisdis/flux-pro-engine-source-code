#include <sys/syscall.h>

#ifdef __aarch64__
__attribute__((always_inline)) inline long __syscall_arm64(long sysno, long arg1 = 0, long arg2 = 0, long arg3 = 0, long arg4 = 0, long arg5 = 0, long arg6 = 0) {
    long ret;
    register long x0 asm("x0") = arg1;
    register long x1 asm("x1") = arg2;
    register long x2 asm("x2") = arg3;
    register long x3 asm("x3") = arg4;
    register long x4 asm("x4") = arg5;
    register long x5 asm("x5") = arg6;
    register long x8 asm("x8") = sysno;
    
    asm volatile(
        "svc #0\n"
        : "=r" (x0)
        : "r" (x0), "r" (x1), "r" (x2), "r" (x3), "r" (x4), "r" (x5), "r" (x8)
        : "memory", "cc"
    );
    
    return x0;
}

#define SYSCALL(sysno, ...) __syscall_arm64(sysno, ##__VA_ARGS__)
#endif
