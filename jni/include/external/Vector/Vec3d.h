#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <cstdio> // std::format এর বদলে snprintf ব্যবহারের জন্য

struct Vec3d {
    union {
        struct {
            double x;
            double y;
            double z;
        };
        double data[3];
    };

    Vec3d() : x(0), y(0), z(0) {}
    Vec3d(double data[]) : x(data[0]), y(data[1]), z(data[2]) {}
    Vec3d(double value) : x(value), y(value), z(value) {}
    Vec3d(double x, double y) : x(x), y(y), z(0) {}
    Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
    
    inline void nullify() { x = y = z = 0.0; }

    // std::format সরিয়ে snprintf ব্যবহার করা হয়েছে
    std::string to_string() { 
        char buf[128];
        snprintf(buf, sizeof(buf), "%.2f, %.2f, %.2f", x, y, z);
        return std::string(buf);
    }
    std::string ToString() { return to_string(); }
    
    Vec3d operator-() const { return Vec3d(-x, -y, -z); }

    Vec3d& operator+=(const double rhs) { x += rhs; y += rhs; z += rhs; return *this; }
    Vec3d& operator-=(const double rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
    Vec3d& operator*=(const double rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
    Vec3d& operator/=(const double rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
    Vec3d& operator+=(const Vec3d rhs)  { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    Vec3d& operator-=(const Vec3d rhs)  { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    Vec3d& operator*=(const Vec3d& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
    Vec3d& operator/=(const Vec3d& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }
    
    friend Vec3d operator+(Vec3d lhs, const double rhs) { return lhs += rhs; }
    friend Vec3d operator-(Vec3d lhs, const double rhs) { return lhs -= rhs; }
    friend Vec3d operator*(Vec3d lhs, const double rhs) { return lhs *= rhs; }
    friend Vec3d operator/(Vec3d lhs, const double rhs) { return lhs /= rhs; }
    friend Vec3d operator+(const double lhs, Vec3d rhs) { return rhs += lhs; }
    friend Vec3d operator-(const double lhs, Vec3d rhs) { return rhs -= lhs; }
    friend Vec3d operator*(const double lhs, Vec3d rhs) { return rhs *= lhs; }
    friend Vec3d operator/(const double lhs, Vec3d rhs) { return rhs /= lhs; }
    friend Vec3d operator+(Vec3d lhs, const Vec3d& rhs) { return lhs += rhs; }
    friend Vec3d operator-(Vec3d lhs, const Vec3d& rhs) { return lhs -= rhs; }
    friend Vec3d operator*(Vec3d lhs, const Vec3d& rhs) { return lhs *= rhs; }
    friend Vec3d operator/(Vec3d lhs, const Vec3d& rhs) { return lhs /= rhs; }
    
    friend bool operator==(const Vec3d lhs, const Vec3d rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
    friend bool operator!=(const Vec3d lhs, const Vec3d rhs) { return !(lhs == rhs); }

    explicit operator bool () const { return x != 0 || y != 0 || z != 0; }
};
