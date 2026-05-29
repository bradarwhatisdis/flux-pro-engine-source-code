#pragma once

#include <cstdint>
#include <string>

#define ptr uintptr_t
extern ptr libmain;

#define F(type, address) (*(type*)(address))
#define M(type, address, ...) ((type(*)(__VA_ARGS__))(address))

#define CUE_PROPERTIES_SPIN F(double, libmain + 0x4e49418)
#define CUE_PROPERTIES_MAX_POWER F(double, libmain + 0x4e49410)

struct Instance {
    ptr instance;

    Instance(ptr instance = 0) : instance(instance) {}
    
    bool isInstanceOf(std::string className) { return instance && F(char*, *(ptr*)instance + 0x10) == className; }

    operator ptr() const { return instance; }
    operator ptr*() const { return (ptr*)instance; }
    
    friend ptr operator+(const Instance& obj, int offset) { return obj.instance + offset; }
    friend ptr operator+(int offset, const Instance& obj) { return obj.instance + offset; }
    friend ptr operator-(const Instance& obj, int offset) { return obj.instance - offset; }
    friend ptr operator-(int offset, const Instance& obj) { return obj.instance - offset; }

    bool operator==(const Instance& other) const { return instance == other.instance; }
    bool operator!=(const Instance& other) const { return instance != other.instance; }

    operator bool() const { return instance != 0; }
};

struct Class : Instance {
    Class(ptr instance = 0) : Instance(instance) {}

    const char* className() const { return F(const char*, instance + 0x10); }
};

#include <type_traits>

template<size_t offset, class T, bool = std::is_base_of<Instance, T>::value>
struct FieldImpl;

template<size_t offset, class T>
struct FieldImpl<offset, T, true> : Instance { // Instance class specialized
    FieldImpl(ptr instance = 0) : Instance(instance) {}

    operator T() { return F(ptr, instance + offset); }
    T operator()() { return F(ptr, instance + offset); }

    void operator()(const T& value) { F(T, instance + offset) = value; }
};

template<size_t offset, class T>
struct FieldImpl<offset, T, false> : Instance {
    FieldImpl(ptr instance = 0) : Instance(instance) {}
    
    operator T&() { return F(T, instance + offset); }
    T& operator()() { return F(T, instance + offset); }

    void operator()(const T& value) { F(T, instance + offset) = value; }
};

template<size_t offset, class T>
struct Field : FieldImpl<offset, T, std::is_base_of<Instance, T>::value> {
    using Base = FieldImpl<offset, T, std::is_base_of<Instance, T>::value>;
    using Base::Base;
};

template<size_t offset, class T>
struct Field<offset, T*> : Instance {
    Field(ptr instance = 0) : Instance(instance) {}
    
    operator T&() { return *(T*)F(ptr, instance + offset); }
    T& operator()() { return *(T*)F(ptr, instance + offset); }
};