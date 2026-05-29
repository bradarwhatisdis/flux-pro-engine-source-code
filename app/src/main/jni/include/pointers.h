#pragma once

#include <csignal>
#include <csetjmp>
#include <iostream>

#include "includes.h"
#include "stacktrace.h"


void SignalTraceHandler(int sig) {
    LOGI("SignalTraceHandler %d", sig);
    log_stacktrace(100);
    exit(1);
}

void SetupSignalTraceHandler() {
    // std::signal(SIGSEGV, SignalTraceHandler); // Segmentation fault
    std::signal(SIGABRT, SignalTraceHandler); // Abort signal
    std::signal(SIGILL,  SignalTraceHandler); // Illegal instruction
    std::signal(SIGFPE,  SignalTraceHandler); // Floating point exception
    std::signal(SIGBUS,  SignalTraceHandler); // Bus error
    std::signal(SIGSYS,  SignalTraceHandler); // Bad system call
    std::signal(SIGTRAP, SignalTraceHandler); // Trace/breakpoint trap
    std::signal(SIGXCPU, SignalTraceHandler); // CPU time limit exceeded
    std::signal(SIGXFSZ, SignalTraceHandler); // File size limit exceeded
}

#define FA(type, address) (type*)address
#define FO(type, address) (*(type*)(address))
#define F(type, address) ({ \
    __typeof__(*(type*)(0)) __result = {}; \
    auto r = (type*)(address); \
    if (r) __result = *r; \
    __result; \
})

#define FS(type, address, value) ({ \
    __typeof__(*(type*)(0)) __result = {}; \
    auto r = (type*)(address); \
    if (r) *r = value; \
    __result; \
})

struct sigaction old_actions[_NSIG];

static sigjmp_buf jump_buffer;
static volatile int jump_buffer_active = 0;
static bool segv_handler_set = false;

extern std::string rpart(const std::string& str, char c);
extern std::string whois(const void* address, bool include_offset);
extern std::string whois(ptr address, bool include_offset);
#include "imgui/inc/persistence.h"
void segv_handler(int sig) {
    LOGI("SEGV");
    
    log_stacktrace();
    if (IsInStack("libwolf.so") || IsInStack("[!dladdr]")) {
        LOGI("Wolf founded");
        // pthread_exit(nullptr);
        sleep(10000);
        return;
    }

    _exit(1);
    // exit(1);

    if (jump_buffer_active) siglongjmp(jump_buffer, 1);
    else LOGI("Jump buffer inactive!");
    
    // return;
    exit(1);
}

void setup_global_segv_handler() {
    struct sigaction new_action{};
    new_action.sa_handler = segv_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGSEGV, &new_action, nullptr);
    segv_handler_set = true;
}

bool is_segv_handler_active() {
    struct sigaction current_action;
    sigaction(SIGSEGV, nullptr, &current_action);
    return current_action.sa_handler == segv_handler;
}

void JUMPTHING() {
    ptr smth = 0;
    jump_buffer_active = 1;
    if (!sigsetjmp(jump_buffer, 1)) {
        auto r = (ptr*)(0x0);
        if (r) smth = *r;
    }
    jump_buffer_active = 0;

    LOGI("smth: %p", smth);
}

#define F(type, address) ({ \
    __typeof__(*(type*)(0)) __result = {}; \
    jump_buffer_active = 1; \
    if (!sigsetjmp(jump_buffer, 1)) { \
        auto r = (type*)(address); \
        if (address && r) __result = *r; \
    } \
    jump_buffer_active = 0; \
    __result; \
})

#define F(type, address) (*(type*)(address))

#define M(type, address, ...) ((type(*)(__VA_ARGS__))(address))

#define MS(type, address, ...) ({ \
    type (*__fn_ptr)(__VA_ARGS__) = NULL; \
    struct sigaction __old_action; \
    setup_segv_handler(&__old_action); \
    if (sigsetjmp(jump_buffer, 1)) { \
        unset_segv_handler(&__old_action); \
    } else { \
        __fn_ptr = (type(*)(__VA_ARGS__))(address); \
        unset_segv_handler(&__old_action); \
    } \
    __fn_ptr; \
})

