#include <cmath>
#include <cstdio>
#include <cstring>

#include <Vector/Vectors.h>

#include "include/includes.h"

bool isInstanceOf(ptr obj, std::string className) { return F(char*, *(ptr*)obj + 0x10) == className; }
char* getClassName(ptr obj) { return F(char*, *(ptr*)obj + 0x10); }

// sub_1C2B0D8
double normalizeDoublePrecision(double value, double negativeThreshold = 0.0, double negativeExtraLen = 0.0, size_t maxLen = 7) {
    // return value;
    if (std::abs(value) >= 10000.0) return std::floor(value);

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "%lf", value);
    size_t strLen = std::strlen(buffer);

    size_t allowedLen = maxLen;
    if (value < negativeThreshold) allowedLen = maxLen + negativeExtraLen;
    if (strLen > allowedLen) buffer[allowedLen] = '\0';

    double result = 0.0;
    std::sscanf(buffer, "%lf", &result);
    return result;
}

auto colors = std::to_array<ImColor>({
    ImColor(1.f, 1.f, 1.f),                         // white
    ImColor(1.f, 1.f, 0.f),                         // yellow
    ImColor(0.f, 0.f, 1.f),                         // blue
    ImColor(1.f, 0.f, 0.f),                         // red
    ImColor(0.501960784314f, 0.f, 0.501960784314f), // purple
    ImColor(1.f, 0.647058823529f, 0.f),             // orange
    ImColor(0.f, 0.501960784314f, 0.f),             // green
    ImColor(0.501960784314f, 0.f, 0.f),             // maroon
    ImColor(0.f, 0.f, 0.f),                         // black
    ImColor(1.f, 1.f, 0.f),                         // yellow
    ImColor(0.f, 0.f, 1.f),                         // blue
    ImColor(1.f, 0.f, 0.f),                         // red
    ImColor(0.501960784314f, 0.f, 0.501960784314f), // purple
    ImColor(1.f, 0.647058823529f, 0.f),             // orange
    ImColor(0.f, 0.501960784314f, 0.f),             // green
    ImColor(0.501960784314f, 0.f, 0.f),             // maroon
});

static_assert(colors.size() == 16);
