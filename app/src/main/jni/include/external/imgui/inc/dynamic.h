#pragma once

#include <map>
#include <string>

using namespace std;

struct DynamicBool : map<const char*, bool> {
    DynamicBool() { }
    
    bool get(const char* key, bool defaultValue = false) {
        auto it = find(key);
        if (it == end()) {
            insert({key, defaultValue});
            return defaultValue;
        }
        return it->second;
    }
    
    // bool operator[](const string &key) { return map<string, bool>::operator[](key); }
};

struct DynamicString : map<const char*, std::string> {
    DynamicString() { }
    
    std::string get(const char* key, std::string defaultValue = "") {
        auto it = find(key);
        if (it == end()) {
            insert({key, defaultValue});
            return defaultValue;
        }
        return it->second;
    }
};

static DynamicBool dynamic_bool;
static DynamicString dynamic_string;

// #define dynadd(key, value) dynamic_bool[key] = value;
