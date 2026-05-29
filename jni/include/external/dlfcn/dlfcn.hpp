#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <dlfcn.h>

#include <sys/system_properties.h>
#include <android/log.h>

struct ctx {
    void *load_addr;
    void *dynstr;
    void *dynsym;
    int nsyms;
    off_t bias;
};

#if defined(__LP64__)
    #define Elf_Ehdr Elf64_Ehdr
    #define Elf_Shdr Elf64_Shdr
    #define Elf_Sym  Elf64_Sym
#else
    #define Elf_Ehdr Elf32_Ehdr
    #define Elf_Shdr Elf32_Shdr
    #define Elf_Sym  Elf32_Sym
#endif

#if defined(__LP64__)
    static const char *const kSystemLibDir = "/system/lib64/";
    static const char *const kOdmLibDir = "/odm/lib64/";
    static const char *const kVendorLibDir = "/vendor/lib64/";
    static const char *const kApexLibDir = "/apex/com.android.runtime/lib64/";
    static const char *const kApexArtNsLibDir = "/apex/com.android.art/lib64/";
#else
    static const char *const kSystemLibDir = "/system/lib/";
    static const char *const kOdmLibDir = "/odm/lib/";
    static const char *const kVendorLibDir = "/vendor/lib/";
    static const char *const kApexLibDir = "/apex/com.android.runtime/lib/";
    static const char *const kApexArtNsLibDir = "/apex/com.android.art/lib/";
#endif

#include "include/obfuscation.h"
EXPORT int fdlclose(void *handle) asm(GET_N(1));
EXPORT void *fake_dlopen_with_path(const char *libpath, int flags) asm(GET_N(2));
EXPORT void *fdlopen(const char *filename, int flags) asm(GET_N(3));
EXPORT void *fdlsym(void *handle, const char *name) asm(GET_N(4));
EXPORT const char *fdlerror() asm(GET_N(5));

