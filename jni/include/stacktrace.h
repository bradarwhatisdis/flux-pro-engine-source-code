#pragma once

#include <execinfo.h>
#include <cxxabi.h>
#include <unwind.h>
#include <dlfcn.h>

#include "logger.h"

struct BacktraceState {
    void** current;
    void** end;
};

static _Unwind_Reason_Code unwind_callback(struct _Unwind_Context* context, void* arg) {
    BacktraceState* state = static_cast<BacktraceState*>(arg);
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
    }
    return _URC_NO_REASON;
}

#define get_frame(i) ({ \
    void** frame = (void**)__builtin_frame_address(0); \
    for (int _idx = 0; _idx < (i) && frame; _idx++) frame = (void**)frame[0]; \
    frame ? ((uintptr_t)frame[1] - 4) : 0; \
})

std::string get_lib_offset(const void* address) {
    Dl_info info;
    if (!dladdr(address, &info)) { char buf[64]; snprintf(buf, sizeof(buf), "[!dladdr](0x%llx)", (unsigned long long)(uintptr_t)address); return std::string(buf); }
    
    uintptr_t offset = (uintptr_t)address - (uintptr_t)info.dli_fbase;
    const char* libname = strrchr(info.dli_fname, '/');
    libname = libname ? libname + 1 : info.dli_fname;
    
    char buf[128];
    snprintf(buf, sizeof(buf), "%s(0x%llx)", libname, (unsigned long long)offset);
    return std::string(buf);
}

std::string get_lib_offset(uintptr_t address) { return get_lib_offset((void*)address); }

uintptr_t get_lib_offset_ptr(const void* address) {
    Dl_info info;
    if (!dladdr(address, &info)) return 0;
    return (uintptr_t)address - (uintptr_t)info.dli_fbase;
}

uintptr_t get_lib_offset_ptr(uintptr_t address) { return get_lib_offset_ptr((void*)address); }

__attribute__((noinline)) std::vector<uintptr_t> v1_get_stacktrace(size_t max_frames = 10, bool skip_caller = false) {
    std::vector<uintptr_t> frames;
    void** frame;
    if (skip_caller) frame = (void**)__builtin_frame_address(2);
    else frame = (void**)__builtin_frame_address(1);
    
    while (frame && frames.size() < max_frames) {
        auto _ = (uintptr_t)frame[1];
        if (_) frames.push_back(_ - 4);
        frame = (void**)frame[0];
    }
    
    return frames;
}

__attribute__((noinline)) std::vector<uintptr_t> get_stacktrace(size_t max_frames = 10, bool skip_caller = false) {
    std::vector<uintptr_t> frames;
    
    // Skip frames: this function + optionally the caller
    size_t skip = skip_caller ? 3 : 2;
    
    // Collect extra frames to account for the ones we'll skip
    size_t total_frames = max_frames + skip;
    void** callstack = new void*[total_frames]();
    BacktraceState state = {callstack, callstack + total_frames};
    _Unwind_Backtrace(unwind_callback, &state);
    
    // Count valid frames
    size_t count = 0;
    while (count < total_frames && callstack[count]) count++;
    
    // Skip unwanted frames and collect up to max_frames
    for (size_t i = skip; i < count && frames.size() < max_frames; i++) {
        frames.push_back((uintptr_t)callstack[i] - 4);
    }
    
    delete[] callstack;
    return frames;
}

bool IsInStack(uintptr_t address, size_t max_frames = 10) {
    auto frames = get_stacktrace(max_frames);
    for (auto frame : frames) if (frame == address) return true;
    return false;
}

extern std::string whois(ptr address, bool include_offset);
bool IsInStack(const std::string& libname, size_t max_frames = 10) {
    auto frames = get_stacktrace(max_frames);
    for (auto frame : frames) if (whois(frame, false) == libname) return true;
    return false;
}

void v1_log_stacktrace(size_t max_frames = 10, bool skip_caller = true) {
    auto frames = v1_get_stacktrace(max_frames, skip_caller);
    
    std::string stacktrace;
    for (int i = frames.size() - 1; i >= 0; i--) {
        if (!stacktrace.empty()) stacktrace += "->";
        stacktrace += get_lib_offset(frames[i]);
    }
    
    LOGI("v1_log_stacktrace: %s", stacktrace.c_str());
}

void log_stacktrace(size_t max_frames = 10, bool skip_caller = true) {
    auto frames = get_stacktrace(max_frames, skip_caller);
    
    std::string stacktrace;
    for (int i = frames.size() - 1; i >= 0; i--) {
        if (!stacktrace.empty()) stacktrace += "->";
        stacktrace += get_lib_offset(frames[i]);
    }
    
    LOGI("log_stacktrace: %s", stacktrace.c_str());
}

void v0_log_stacktrace(size_t max_frames = 10) {
    void** callstack = new void*[max_frames]();
    BacktraceState state = {callstack, callstack + max_frames};
    _Unwind_Backtrace(unwind_callback, &state);
    
    // Skip the first frame (this function)
    size_t count = 0;
    while (count < max_frames && callstack[count]) count++;
    
    if (count > 1) {
        // Shift all frames down by one
        for (size_t i = 1; i < count; i++) {
            callstack[i-1] = callstack[i];
        }
        count--;
    }
    
    std::string stacktrace;
    // Print in reverse order (oldest call first)
    for (int i = count - 1; i >= 0; i--) {
        if (!stacktrace.empty()) stacktrace += "->";
        stacktrace += get_lib_offset((void*)((uintptr_t)callstack[i] - 4));
    }
    
    LOGI("v0_log_stacktrace: %s", stacktrace.c_str());
    delete[] callstack;
}
