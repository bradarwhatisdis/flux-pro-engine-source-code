#pragma once

#include <cstdint>

#include "Types.h"

template<class T>
struct NSArray {
    NSArray() : Data(nullptr), Count(0), Max(0) {}

    T& operator[](int i) { return i >= 0 ? Data[i] : Data[Count + i]; }
    const T& operator[](int i) const { return i >= 0 ? Data[i] : Data[Count + i]; }

    T* begin() { return Data; }
    const T* begin() const { return Data; }
    T* end() { return Data + Count; }
    const T* end() const { return Data + Count; }

    operator bool () const { return Count > 0 && Data; }

    uintptr_t Class;
    uintptr_t Count;
    uintptr_t Max;
    T* Data;
};

template<class T>
struct PNSArray {
    PNSArray() : Data(nullptr), Count(0), Max(0) {}

    T operator[](int i) { return i >= 0 ? Data[i] : Count + i >= 0 ? Data[Count + i] : 0; }
    const T operator[](int i) const { return i >= 0 ? Data[i] : Count + i >= 0 ? Data[Count + i] : 0; }

    uintptr_t* begin() { return Data; }
    const uintptr_t* begin() const { return Data; }
    uintptr_t* end() { return Data + Count; }
    const uintptr_t* end() const { return Data + Count; }

    operator bool () const { return Count > 0 && Data; }

    uintptr_t Class;
    uintptr_t Count;
    uintptr_t Max;
    uintptr_t* Data;
};

template<class T, size_t Count>
struct Array {
    Array() : Data(nullptr) {}

    T operator[](int i) { return i >= 0 ? Data[i] : Count + i >= 0 ? Data[Count + i] : 0; }
    const T operator[](int i) const { return i >= 0 ? Data[i] : Count + i >= 0 ? Data[Count + i] : 0; }

    uintptr_t* begin() { return Data; }
    const uintptr_t* begin() const { return Data; }
    uintptr_t* end() { return Data + Count; }
    const uintptr_t* end() const { return Data + Count; }

    uintptr_t* Data;
};
