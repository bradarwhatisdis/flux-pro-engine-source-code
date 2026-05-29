#pragma once

#include "include/random_names.h"

#define IMPL_1(cnt) \
    EXPORT NOINLINE void GET_L(cnt)() { \
        volatile int x = O(0); V(x)++; \
        volatile int i = 0; i++; \
    }

#define IMPL_2(cnt) \
    EXPORT NOINLINE void GET_L(cnt)() { \
        volatile int x = O(0); V(x)--; \
        volatile int i = 0; i--; \
    }

#define IMPL_3(cnt) \
    EXPORT NOINLINE void* GET_L(cnt)() { \
        volatile int i = O(10); \
        OBF_BEGIN WHILE(V(i) > 0) V(i)--; ENDWHILE OBF_END \
        volatile int x = 10; while(x > 0) x--; \
    }

#define IMPL_4(cnt) \
    EXPORT NOINLINE void GET_L(cnt)() { \
        volatile int x = O(1), y = O(1); V(x) += V(y); \
        volatile int a = 1, b = 1; a += b; \
    }

#define IMPL_5(cnt) \
    EXPORT NOINLINE void GET_L(cnt)() { \
        volatile int x = O(1), y = O(1); V(x) -= V(y); \
        __asm__ __volatile__("nop"); \
    }

#define DEFINE_DECOY_1 IMPL_1(__COUNTER__)
#define DEFINE_DECOY_2 IMPL_2(__COUNTER__)
#define DEFINE_DECOY_3 IMPL_3(__COUNTER__)
#define DEFINE_DECOY_4 IMPL_4(__COUNTER__)
#define DEFINE_DECOY_5 IMPL_5(__COUNTER__)

#define DEFINE_DECOYS_3 DEFINE_DECOY_5 DEFINE_DECOY_2 DEFINE_DECOY_4
#define DEFINE_DECOYS_5 DEFINE_DECOYS_3 DEFINE_DECOY_3 DEFINE_DECOY_1
#define DEFINE_DECOYS_10 DEFINE_DECOYS_5 DEFINE_DECOYS_3 DEFINE_DECOY_4 DEFINE_DECOY_2
#define DEFINE_DECOYS_25 DEFINE_DECOYS_10 DEFINE_DECOYS_10 DEFINE_DECOY_1 DEFINE_DECOY_2 DEFINE_DECOY_3 DEFINE_DECOY_4 DEFINE_DECOY_5
#define DEFINE_DECOYS_50 DEFINE_DECOYS_25 DEFINE_DECOYS_25
#define DEFINE_DECOYS_100 DEFINE_DECOYS_50 DEFINE_DECOYS_50
