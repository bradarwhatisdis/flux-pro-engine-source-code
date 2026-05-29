#include <And64InlineHook/And64InlineHook.hpp>
#include <Substrate/SubstrateHook.h>
#include <dlfcn/dlfcn.hpp>

#if defined(__aarch64__)
    #define HOOK(a, r) A64HookFunction((void*)(a), (void*)r, (void**)&_##r)
    #define HOOKI(a, r, o) A64HookFunction((void*)(a), (void*)r, (void**)&o)
#else
    #define HOOK(a, r) MSHookFunction((void*)(a), (void*)r, (void**)&_##r)
    #define HOOKI(a, r, o) MSHookFunction((void*)(a), (void*)r, (void**)&o)
#endif

#define HOOKS(l, s, r) ([]() { \
    void* pl = fdlopen(l, 4); \
    if (!pl) { LOGI("!dlopen %s", l); return; }; \
    void* ps = fdlsym(pl, s); \
    if (!ps) { LOGI("!dlsym %s %s", l, s); return; }; \
    fdlclose(pl); \
    HOOK(ps, r); \
})();

#define DEFINES(type, func, ...) static type (*_##func)(__VA_ARGS__); static type func(__VA_ARGS__)
#define DEFINE(type, func, ...) type (*_##func)(__VA_ARGS__); type func(__VA_ARGS__)
