#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <cstdio> // std::format এর বদলে snprintf ব্যবহারের জন্য

struct Vec2d {
    union {
        struct {
            double x;
            double y;
        };
        double data[2];
    };
    
    Vec2d() : x(0), y(0) {}
    Vec2d(double data[]) : x(data[0]), y(data[1]) {}
    Vec2d(double value) : x(value), y(value) {}
    Vec2d(double x, double y) : x(x), y(y) {}

    inline double square() const { return (x * x + y * y); }
    inline void nullify() { x = y = 0.0; }

    // std::format সরিয়ে snprintf ব্যবহার করা হয়েছে
    std::string to_string() { 
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f, %.2f", x, y);
        return std::string(buf); 
    }
    std::string ToString() { return to_string(); }
    
    Vec2d operator-() const { return Vec2d(-x, -y); }

    Vec2d& operator+=(const double rhs) { x += rhs; y += rhs; return *this; }
    Vec2d& operator-=(const double rhs) { x -= rhs; y -= rhs; return *this; }
    Vec2d& operator*=(const double rhs) { x *= rhs; y *= rhs; return *this; }
    Vec2d& operator/=(const double rhs) { x /= rhs; y /= rhs; return *this; }
    Vec2d& operator+=(const Vec2d rhs)  { x += rhs.x; y += rhs.y; return *this; }
    Vec2d& operator-=(const Vec2d rhs)  { x -= rhs.x; y -= rhs.y; return *this; }
    Vec2d& operator*=(const Vec2d& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
    Vec2d& operator/=(const Vec2d& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
    
    friend Vec2d operator+(Vec2d lhs, const double rhs) { return lhs += rhs; }
    friend Vec2d operator-(Vec2d lhs, const double rhs) { return lhs -= rhs; }
    friend Vec2d operator*(Vec2d lhs, const double rhs) { return lhs *= rhs; }
    friend Vec2d operator/(Vec2d lhs, const double rhs) { return lhs /= rhs; }
    friend Vec2d operator+(const double lhs, Vec2d rhs) { return rhs += lhs; }
    friend Vec2d operator-(const double lhs, Vec2d rhs) { return rhs -= lhs; }
    friend Vec2d operator*(const double lhs, Vec2d rhs) { return rhs *= lhs; }
    friend Vec2d operator/(const double lhs, Vec2d rhs) { return rhs /= lhs; }
    friend Vec2d operator+(Vec2d lhs, const Vec2d& rhs) { return lhs += rhs; }
    friend Vec2d operator-(Vec2d lhs, const Vec2d& rhs) { return lhs -= rhs; }
    friend Vec2d operator*(Vec2d lhs, const Vec2d& rhs) { return lhs *= rhs; }
    friend Vec2d operator/(Vec2d lhs, const Vec2d& rhs) { return lhs /= rhs; }

    friend bool operator==(const Vec2d& lhs, const Vec2d& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator!=(const Vec2d& lhs, const Vec2d& rhs) { return !(lhs == rhs); }

    explicit operator bool () const { return x != 0 || y != 0; }
};
