#pragma once

#include <android/log.h>
#define MY_LOG_TAG "angousana"

// #define NDEBUG

// #ifdef NDEBUG
//     // Release build - void logging macros
//     #define LOGI(...)
//     #define LOGD(...)
//     #define LOGW(...)
//     #define LOGE(...)
// #else
    // Debug build - normal logging macros
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, MY_LOG_TAG, __VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MY_LOG_TAG, __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, MY_LOG_TAG, __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, MY_LOG_TAG, __VA_ARGS__)
// #endif

#define ifl(cond) if ([&](){ bool b = (cond); if (b) LOGI(#cond); return b; }())
// #define ifln(cond) if ([&](){ bool b = (cond); if (!b) LOGI("!("#cond")"); return b; }())
