#pragma once

#include <string>
static std::string PACKAGE_NAME;

#include "Vector/Vectors.h"
#include <cstdint>
static uintptr_t libmain;
static uintptr_t libanogs;
static uintptr_t libintl;
static bool bOnInputEvent = false;
static bool bImguiSetup = false;
static int Width, Height, Orientation;
static Vector2 screenCenter;

#include <fstream>
#include <sstream>
#include <string>
#include <initializer_list>

#include <cstring>
#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <thread>
#include <set>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include <chrono>
#include <deque>

#define ptr uintptr_t
#define DWORD uintptr_t
#define ulong uint64_t
#define dword uint32_t
#define qword uint64_t

#include "stacktrace.h"
#include "pointers.h"

using namespace std;

#include "logger.h"

#define I(...) { LOGI(__VA_ARGS__); }
#define E(e, ...) { LOGD(__VA_ARGS__); e; }
#define R(i, ...) { LOGD(__VA_ARGS__); return i; }

#define IC(xyz) if (xyz) { \
    LOGI("%s:%d %s", rpart(__FILE__, '/').c_str(), __LINE__, #xyz); \
    continue; \
}

#define IDC(xyz) if (xyz) { \
    if (dynamic_bool["LogContinue"]) LOGI("%s:%d %s", rpart(__FILE__, '/').c_str(), __LINE__, #xyz); \
    continue; \
}

#define IR(xyz, ...) if (xyz) { \
    LOGI("%s:%d %s", rpart(__FILE__, '/').c_str(), __LINE__, #xyz); \
    return __VA_ARGS__; \
}

#define IDR(xyz, ...) if (xyz) { \
    if (dynamic_bool["LogReturn"]) LOGI("%s:%d %s", rpart(__FILE__, '/').c_str(), __LINE__, #xyz); \
    return __VA_ARGS__; \
}

#define ARGB(a, r, g, b) r << 0 | g << 8 | b << 16 | a << 24

#define WHITE              ImColor(255, 255, 255)
#define RED                ImColor(255, 0, 0)
#define GREEN              ImColor(0, 255, 0)
#define LIME               ImColor(0, 255, 0)
#define BLUE               ImColor(0, 0, 255)
#define BLACK              ImColor(0, 0, 0)
#define PURPLE             ImColor(128, 0, 128)
#define GREY               ImColor(128, 128, 128)
#define YELLOW             ImColor(255, 255, 0)
#define ORANGE             ImColor(255, 165, 0)
#define DARKGREEN          ImColor(0, 100, 0)
#define PINK               ImColor(255, 192, 203)
#define BROWN              ImColor(165, 42, 42)
#define CYAN               ImColor(0, 255, 255)

#define floop(s, e) for (int fi=s; fi<e; fi++)

void* void_func_wrapper(void* arg) {
    M(void, arg)();
    return 0;
}

void pthread(void (*func)(void)) {
    pthread_t x;
    pthread_create(&x, 0, void_func_wrapper, (void*)func);
    pthread_detach(x);
}

void sleepm(int milliseconds) {std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));}

#include <sys/mman.h>
bool EditPerm(void* addr, size_t len, int PERMISSION) {return (mprotect((void*)((uintptr_t)addr&~(uintptr_t)((sysconf(_SC_PAGE_SIZE))-1)),((((uintptr_t)(uintptr_t)addr+len-1&~(uintptr_t)((sysconf(_SC_PAGE_SIZE))-1)))-((uintptr_t)addr&~(uintptr_t)((sysconf(_SC_PAGE_SIZE))-1))+(sysconf(_SC_PAGE_SIZE))),PERMISSION)!=-1);}
bool EditMemory(ptr addr, size_t len, const void *data) {EditPerm((void*)addr, len, (PROT_READ | PROT_WRITE | PROT_EXEC)); bool x = (memcpy((void*)addr, data, len)!=NULL); EditPerm((void*)addr, len, (PROT_READ | PROT_EXEC)); return x;}

#define DEFINED(type, name, ...) int __##name = -1; type (*_##name)(); type name()

DEFINED(uint, LOGS) { static int _I_ = 0; LOGI("LOGS: %i", _I_++); log_stacktrace(); return 0; }

DEFINED(uint, SLEEP) { static int _I_ = 0; LOGI("SLEEP: %i", _I_++); sleep(1000); return 0; }

DEFINED(uint, SVOID) { return 0; }
DEFINED(uint, VOID) { static int _I_ = 0; LOGI("VOID: %i", _I_++); return 0; }

DEFINED(bool, STRUE) { return true; }
DEFINED(bool, TRUE) { static int _I_ = 0; LOGI("TRUE: %i", _I_++); return true; }
DEFINED(bool, SFALSE) { return false; }
DEFINED(bool, FALSE) { static int _I_ = 0; LOGI("FALSE: %i", _I_++); return false; }

int IMAX() {static int _I_ = 0;LOGI("IMAX: %i", _I_++);return 100000;}
int I0() {static int _I_ = 0;LOGI("I0: %i", _I_++);return 0;}
int I10() {static int _I_ = 0;LOGI("I10: %i", _I_++);return 10;}
int I1000() {static int _I_ = 0;LOGI("I1000: %i", _I_++);return 1000;}
float FMAX() {static int _I_ = 0;LOGI("FMAX: %i", _I_++);return 100000.0f;}
float F1000() {static int _I_ = 0;LOGI("F1000: %i", _I_++);return 1000.0f;}
float F100() {static int _I_ = 0;LOGI("F100: %i", _I_++);return 100.0f;}
float F10() {static int _I_ = 0;LOGI("F10: %i", _I_++);return 10.0f;}
float F0() {static int _I_ = 0;LOGI("F0: %i", _I_++);return 0.0f;}
float SF0() { return 0.0f; }
float SF10() { return 10.0f; }
float SF100() { return 100.0f; }
double DMAX() {static int _I_ = 0;LOGI("DMAX: %i", _I_++);return 100000.0f;}
// vector<int> V3() {static int _I_ = 0;LOGI("V3: %i", _I_++);return {0,0,0};}
// vector<int> V2() {static int _I_ = 0;LOGI("V2: %i", _I_++);return {0,0};}

#define BT(addr) HOOKN(addr, TRUE);
#define BF(addr) HOOKN(addr, FALSE);
#define V(addr) HOOKN(addr, VOID);

#define IM(addr) HOOKN(addr, IMAX);

#define SV(addr) HOOKN(addr, SVOID);
#define ST(addr) HOOKN(addr, STRUE);
#define SF(addr) HOOKN(addr, SFALSE);

u_long htol(string strx) {const char* str = strx.c_str(); u_long res = 0; char c; while ((c = *str++)) {char v = (c & 0xF) + (c >> 6) | ((c >> 3) & 0x8); res = (res << 4) | (u_long) v;} return res;}
ptr absoluteAddress(const char *libraryName, uintptr_t offset=0) {
    bool found=false;
    LOGI("Searching %s until found...", libraryName);
    while (!found) {
        ifstream maps("/proc/self/maps");
        string map;
        while (getline(maps, map)) {
            if (strstr(map.c_str(), libraryName)) {
                LOGI("Library found! Exiting loop...");
                istringstream line(map);
                string start;
                getline(line, start, '-');
                return htol(start)+offset;
            }
        }
        sleep(1);
    }
    return 0;
}
template<typename... Args> void absoluteAddress(Args... args) {
    const char* arr[] = {args...};
    for (size_t i = 0; i < sizeof...(args); i++) {
        absoluteAddress(arr[i]);
    }
}

#include <regex>

std::regex r8BPbase("/data/.*./com\\.miniclip\\.eightballpool/[0-9A-Z]{32}/[0-9a-z]{12}/[0-9a-z]{20}/");
ptr get8BPbase(uintptr_t offset=0) {
    bool found = false;
    LOGI("Searching 8BP base until found...");
    while (!found) {
        ifstream maps("/proc/self/maps");
        string line;
        std::map<string, pair<ptr, size_t>> libs;

        while (getline(maps, line)) {
            if (line.find("/com.miniclip.eightballpool/") != string::npos) {
                istringstream iss(line);
                string range, perms, offsetStr, dev, inode;
                iss >> range >> perms >> offsetStr >> dev >> inode;
                
                string rest;
                getline(iss, rest);
                size_t first = rest.find_first_not_of(" \t");
                if (first == string::npos) continue;
                string path = rest.substr(first);
                
                size_t dash = range.find('-');
                if (dash != string::npos) {
                    ptr base = htol(range.substr(0, dash));
                    ptr end = htol(range.substr(dash + 1));
                    size_t size = end - base;
                    
                    if (libs.find(path) == libs.end()) {
                        libs[path] = {base, size};
                    } else {
                        libs[path].second += size;
                        if (base < libs[path].first) libs[path].first = base;
                    }
                }
            }
        }

        ptr largestBase = 0;
        size_t largestSize = 0;

        for (auto const& entry : libs) {
            if (entry.second.second > largestSize) {
                largestSize = entry.second.second;
                largestBase = entry.second.first;
            }
        }

        if (largestBase != 0) {
            unsigned char* magic = (unsigned char*)largestBase;
            if (magic[0] == 0x7F && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F') {
                LOGI("Library found! Base: %p, Size: %zu", (void*)largestBase, largestSize);
                return largestBase + offset;
            } else {
                LOGW("Found matching path but invalid ELF header at %p (bytes: %02x %02x %02x %02x)", (void*)largestBase, magic[0], magic[1], magic[2], magic[3]);
            }
        }
        sleep(1);
    }
    return 0;
}
ptr getWolfBase(uintptr_t offset=0) {
    bool found = false;
    LOGI("Searching Wolf base until found...");
    while (!found) {
        ifstream maps("/proc/self/maps");
        vector<string> lines;
        string line;
        
        // Read all lines first
        while (getline(maps, line)) {
            lines.push_back(line);
        }
        
        // Check for three consecutive matching lines
        for (size_t i = 0; i + 2 < lines.size(); ++i) {
            const string& map = lines[i];
            const string& map2 = lines[i+1];
            const string& map3 = lines[i+2];
            
            if (strstr(map.c_str(), " rwxp ") && !strstr(map.c_str(), "          ") &&
                strstr(map2.c_str(), " rwxp ") && !strstr(map2.c_str(), "          ") &&
                strstr(map3.c_str(), " rwxp ") && !strstr(map3.c_str(), "          ")) {
                LOGI("Library found with three consecutive matching segments!");
                istringstream line_stream(map);
                string start;
                getline(line_stream, start, '-');
                return htol(start)+offset;
            }
        }
        
        sleep(1);
    }
    return 0;
}

#define FOR(item, list) if (auto* __l = (list)) \
    if (auto __s = __l->getSize()) if (auto* __items = __l->getItems()) \
        for (int __i = 0; __i < __s; ++__i) if (auto item = __items[__i])

#define FORA(item, array) if (auto* __l = (array)) \
    if (auto __s = __l->getLength()) if (auto* __items = __l->getPointer()) \
        for (int __i = 0; __i < __s; ++__i) if (auto item = __items[__i])

#define A(x) void* x() { LOGI(#x); return 0; }
#define B(x) HOOKNI(fdlsym(libc, #x), x);

#include <sys/stat.h>

static bool create_directory_recursive(const char* path) {
    char tmp[256];
    char* p = nullptr;
    size_t len;
    
    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
        
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            *p = '/';
        }
    }
    
    return mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 || errno == EEXIST;
}

#define D1(func) \
    DEFINES(void*, func, void* v1) { \
        LOGI("0" #func); \
        return _##func(v1); \
    }
#define D2(func) \
    DEFINES(void*, func, void* v1, void* v2) { \
        LOGI("O" #func); \
        return _##func(v1, v2); \
    }

#define D(addr) HOOKI(libmain + 0##addr, addr)
#define H(addr, func) HOOKI(libmain + addr, func)

#define HOOK(pointer, func) ({ \
    bool _hooked = false; \
    if (pointer && HMMADR((ptr)pointer)) { \
        if (!_##func && pointer != func) { \
            sleep(1); \
            _##func = pointer; \
            pointer = func; \
            _hooked = true; \
        } else _hooked = true; \
    } \
    _hooked; \
})

#include <string>

bool startswith(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) return false;
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool endswith(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    
    if (suffix_len > str_len) return false;
    
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}


#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>

std::string lpath(const std::string& libname) {
    std::ifstream maps("/proc/self/maps");
    std::string line;
    
    while (std::getline(maps, line)) {
        std::string ends = "/" + libname;
        if (endswith(line.c_str(), ends.c_str())) {
            size_t last_space = line.rfind(' ');
            if (last_space != std::string::npos) {
                std::string path = line.substr(last_space + 1);
                path.erase(std::remove(path.begin(), path.end(), '\n'), path.end());
                path.erase(std::remove(path.begin(), path.end(), '\r'), path.end());
                if (!path.empty()) return path;
            }
        }
    }
    
    R("", "no %s found", libname.c_str());
}

size_t fsize(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) return st.st_size;
    R(0, "get stat failed");
}

size_t lsize(const std::string& libname) {
    auto libpath = lpath(libname);
    if (libpath.empty()) R(0, "libpath.empty()");
    return fsize(libpath);
}

const char* getcmdline() {
    static char buf[4096];
    
    int fd = open("/proc/self/cmdline", O_RDONLY);
    if (fd == -1) return nullptr;
    
    ssize_t n = read(fd, buf, sizeof(buf) - 1); close(fd);
    if (n <= 0) return nullptr;

    return buf;
}

string rpart(const std::string& str, char c) {
    size_t last_slash = str.rfind(c);
    if (last_slash == string::npos) return str;
    return str.substr(last_slash + 1);
}

std::vector<std::string> split(const std::string &s, const char &delim = ' ') {
    std::vector<std::string> tokens;
    size_t lastPos = s.find_first_not_of(delim, 0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != std::string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delim, pos);
        pos = s.find(delim, lastPos);
    }
    return tokens;
}

#define isin strstr
#define iseq strstr
#define streq !strcmp

#include <dlfcn.h>
string whois(const void* address, bool include_offset = false) {
    Dl_info info;
    if (!dladdr(address, &info)) { if (!include_offset) return "[!dladdr]"; char buf[64]; snprintf(buf, sizeof(buf), "[!dladdr](0x%llx)", (unsigned long long)(uintptr_t)address); return std::string(buf); }

    auto libname = rpart(info.dli_fname, '/');
    if (!include_offset) return libname;

    uintptr_t offset = (uintptr_t)address - (uintptr_t)info.dli_fbase;

    char buf[128]; snprintf(buf, sizeof(buf), "%s(0x%llx)", libname.c_str(), (unsigned long long)offset); return std::string(buf);
}

string whois(ptr address, bool include_offset = false) { return whois((void*)address, include_offset); }

#define CONC(...) ([]() -> const char* { \
    static std::string result; \
    result.clear(); \
    const char* parts[] = {__VA_ARGS__}; \
    for (const char* s : parts) result += s; \
    return result.c_str(); \
})()

#define SCONC(...) ([]() -> std::string { \
    std::string result; \
    const char* parts[] = {__VA_ARGS__}; \
    for (const char* s : parts) result += s; \
    return result; \
})()
/*
#define CONC(...) \
    ([](const std::initializer_list<const char*>& parts) -> const char* { \
        static std::string result; \
        result.clear(); \
        for (const char* s : parts) result += s; \
        return result.c_str(); \
    })({__VA_ARGS__})

#define SCONC(...) \
    ([](const std::initializer_list<const char*>& parts) { \
        std::string result; \
        for (const char* s : parts) result += s; \
        return result; \
    })({__VA_ARGS__})
*/

#define X(r, s, f, _f) xhook_register(".*/lib" r "\\.so$", s, (void*)f, (void**)&_f)
#define XN(r, s, f) xhook_register(".*/lib" r "\\.so$", s, (void*)f, NULL)

bool is_aligned(uintptr_t address, size_t alignment) {
    return (address & (alignment - 1)) == 0;
}

#define TRIGGER_SAFEGUARD { \
    LOGI("%s:%d %s", rpart(__FILE__, '/').c_str(), __LINE__, "SAFEGUARD TRIGGERED, EXITING!"); \
    _exit(0); \
}
