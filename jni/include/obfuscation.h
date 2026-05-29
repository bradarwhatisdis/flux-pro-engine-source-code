#pragma once

// #include "obfusheader/obfusheader.h"
#include "obfy/instr.h"
#include "oxorany/oxorany.h"
#include "random_names.h"

#define INLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))

#define EXPORT extern "C" __attribute__((visibility("default")))
#define EXPORT_MANGLED extern "C"

static INLINE void inline_strcpy(char* dest, const char* src) {
    while ((*dest++ = *src++));
}

static INLINE unsigned long inline_strtoul(const char* nptr, char** endptr) {
    unsigned long result = 0;
    while (*nptr) {
        char c = *nptr++;
        if (c >= '0' && c <= '9') result = result * 16 + (c - '0');
        else if (c >= 'a' && c <= 'f') result = result * 16 + (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') result = result * 16 + (c - 'A' + 10);
        else break;
    } if (endptr) *endptr = (char*)nptr;
    return result;
}

static INLINE size_t inline_strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s);
    return (s - str);
}

static INLINE char* inline_strncat(char* dest, const char* src, size_t n) {
    char* p = dest;
    while (*p != 0) p++;
    while (n > 0 && *src != 0) {
        *p++ = *src++;
        n--;
    } *p = 0;
    return dest;
}

static INLINE int inline_strcmp(const char* s1, const char* s2) {
OBF_BEGIN
    WHILE (*s1 == *s2++) IF (*s1++ == 0) RETURN(0); ENDIF ENDWHILE
    RETURN(*(unsigned char*)s1 - *(unsigned char*)--s2);
OBF_END
}

/* static INLINE int inline_strcmp(const char* s1, const char* s2) {
    while (*s1 == *s2++) if (*s1++ == 0) return 0;
    return *(unsigned char*)s1 - *(unsigned char*)--s2;
} */

static INLINE int inline_strncmp(const char* s1, const char* s2, size_t n) {
    unsigned char u1, u2;
    while (n-- > 0) {
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;
        if (u1 != u2) return u1 - u2;
        if (u1 == '\0') return 0;
    } return 0;
}

static INLINE char* inline_strstr(const char* s, const char* find) {
    char c, sc;
    size_t len;
    if ((c = *find++) != 0) {
        len = inline_strlen(find);
        do {
            do {
                if ((sc = *s++) == 0) return NULL;
            } while (sc != c);
        } while (inline_strncmp(s, find, len) != 0);
        s--;
    }
    return (char*)s;
}