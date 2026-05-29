#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <cstdio> // std::format এর বদলে snprintf ব্যবহারের জন্য


struct Vector2d
{
    union
    {
        struct
        {
            double x;
            double y;
        };
        double data[2];
    };


    /**
     * Constructors.
     */
    inline Vector2d();
    inline Vector2d(double data[]);
    inline Vector2d(double value);
    inline Vector2d(double x, double y);


    /**
     * Constants for common vectors.
     */
    static inline Vector2d Zero();
    static inline Vector2d One();
    static inline Vector2d Right();
    static inline Vector2d Left();
    static inline Vector2d Up();
    static inline Vector2d Down();


    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static inline float Angle(Vector2d a, Vector2d b);

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static inline Vector2d ClampMagnitude(Vector2d vector, float maxLength);

    /**
     * Returns the component of a in the direction of b (scalar projection).
     * @param a: The target vector.
     * @param b: The vector being compared against.
     * @return: A scalar value.
     */
    static inline float Component(Vector2d a, Vector2d b);

    /**
     * Returns the distance between a and b.
     * @param a: The first point.
     * @param b: The second point.
     * @return: A scalar value.
     */
    static inline float Distance(Vector2d a, Vector2d b);

    /**
     * Returns the dot product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Vector2d lhs, Vector2d rhs);

    /**
     * Converts a polar representation of a vector into cartesian
     * coordinates.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     * @return: A new vector.
     */
    static inline Vector2d FromPolar(float rad, float theta);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static inline Vector2d Lerp(Vector2d a, Vector2d b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static inline Vector2d LerpUnclamped(Vector2d a, Vector2d b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vector2d v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector2d Max(Vector2d a, Vector2d b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector2d Min(Vector2d a, Vector2d b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vector2d MoveTowards(Vector2d current, Vector2d target,
                               float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static inline Vector2d Normalized(Vector2d v);

    /**
     * Creates a new coordinate system out of the two vectors.
     * Normalizes "normal" and normalizes "tangent" and makes it orthogonal to
     * "normal"..
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     */
    static inline void OrthoNormalize(Vector2d &normal, Vector2d &tangent);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector2d Project(Vector2d a, Vector2d b);

    /**
     * Returns a vector reflected about the provided line.
     * This behaves as if there is a plane with the line as its normal, and the
     * vector comes in and bounces off this plane.
     * @param vector: The vector traveling inward at the imaginary plane.
     * @param line: The line about which to reflect.
     * @return: A new vector pointing outward from the imaginary plane.
     */
    static inline Vector2d Reflect(Vector2d vector, Vector2d line);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector2d Reject(Vector2d a, Vector2d b);

    /**
     * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
     * This treats the vectors as directions and will linearly interpolate
     * between their magnitudes by "maxMagnitudeDelta". This function does not
     * overshoot. If a negative delta is supplied, it will rotate away from
     * "target" until it is pointing the opposite direction, but will not
     * overshoot that either.
     * @param current: The starting direction.
     * @param target: The destination direction.
     * @param maxRadiansDelta: The maximum number of radians to rotate.
     * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
     * @return: A new vector.
     */
    static inline Vector2d RotateTowards(Vector2d current, Vector2d target,
                                 float maxRadiansDelta,
                                 float maxMagnitudeDelta);

    /**
     * Multiplies two vectors component-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vector2d Scale(Vector2d a, Vector2d b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector2d Slerp(Vector2d a, Vector2d b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector2d SlerpUnclamped(Vector2d a, Vector2d b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vector2d v);

    /**
     * Calculates the polar coordinate space representation of a vector.
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle from the X axis.
     */
    static inline void ToPolar(Vector2d vector, float &rad, float &theta);


        // std::format এর বদলে snprintf ব্যবহার করা হয়েছে r24 এর জন্য
    std::string to_string() { 
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f, %.2f", x, y);
        return std::string(buf);
    }
    std::string ToString() { return to_string(); }


    explicit operator bool () const { return x != 0 || y != 0; }

    /**
     * Operator overloading.
     */
    inline struct Vector2d& operator+=(const float rhs);
    inline struct Vector2d& operator-=(const float rhs);
    inline struct Vector2d& operator*=(const float rhs);
    inline struct Vector2d& operator/=(const float rhs);
    inline struct Vector2d& operator+=(const Vector2d rhs);
    inline struct Vector2d& operator-=(const Vector2d rhs);
};

inline Vector2d operator-(Vector2d rhs);
inline Vector2d operator+(Vector2d lhs, const float rhs);
inline Vector2d operator-(Vector2d lhs, const float rhs);
inline Vector2d operator*(Vector2d lhs, const float rhs);
inline Vector2d operator/(Vector2d lhs, const float rhs);
inline Vector2d operator+(const float lhs, Vector2d rhs);
inline Vector2d operator-(const float lhs, Vector2d rhs);
inline Vector2d operator*(const float lhs, Vector2d rhs);
inline Vector2d operator/(const float lhs, Vector2d rhs);
inline Vector2d operator+(Vector2d lhs, const Vector2d rhs);
inline Vector2d operator-(Vector2d lhs, const Vector2d rhs);
inline bool operator==(const Vector2d lhs, const Vector2d rhs);
inline bool operator!=(const Vector2d lhs, const Vector2d rhs);



/*******************************************************************************
 * Implementation
 */

Vector2d::Vector2d() : x(0), y(0) {}
Vector2d::Vector2d(double data[]) : x(data[0]), y(data[1]) {}
Vector2d::Vector2d(double value) : x(value), y(value) {}
Vector2d::Vector2d(double x, double y) : x(x), y(y) {}


Vector2d Vector2d::Zero() { return Vector2d(0, 0); }
Vector2d Vector2d::One() { return Vector2d(1, 1); }
Vector2d Vector2d::Right() { return Vector2d(1, 0); }
Vector2d Vector2d::Left() { return Vector2d(-1, 0); }
Vector2d Vector2d::Up() { return Vector2d(0, 1); }
Vector2d Vector2d::Down() { return Vector2d(0, -1); }


float Vector2d::Angle(Vector2d a, Vector2d b)
{
    float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    return acos(v);
}

Vector2d Vector2d::ClampMagnitude(Vector2d vector, float maxLength)
{
    float length = Magnitude(vector);
    if (length > maxLength)
        vector *= maxLength / length;
    return vector;
}

float Vector2d::Component(Vector2d a, Vector2d b)
{
    return Dot(a, b) / Magnitude(b);
}

float Vector2d::Distance(Vector2d a, Vector2d b)
{
    return Vector2d::Magnitude(a - b);
}

float Vector2d::Dot(Vector2d lhs, Vector2d rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Vector2d Vector2d::FromPolar(float rad, float theta)
{
    Vector2d v;
    v.x = rad * cos(theta);
    v.y = rad * sin(theta);
    return v;
}

Vector2d Vector2d::Lerp(Vector2d a, Vector2d b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return LerpUnclamped(a, b, t);
}

Vector2d Vector2d::LerpUnclamped(Vector2d a, Vector2d b, float t)
{
    return (b - a) * t + a;
}

float Vector2d::Magnitude(Vector2d v)
{
    return sqrt(SqrMagnitude(v));
}

Vector2d Vector2d::Max(Vector2d a, Vector2d b)
{
    float x = a.x > b.x ? a.x : b.x;
    float y = a.y > b.y ? a.y : b.y;
    return Vector2d(x, y);
}

Vector2d Vector2d::Min(Vector2d a, Vector2d b)
{
    float x = a.x > b.x ? b.x : a.x;
    float y = a.y > b.y ? b.y : a.y;
    return Vector2d(x, y);
}

Vector2d Vector2d::MoveTowards(Vector2d current, Vector2d target,
                             float maxDistanceDelta)
{
    Vector2d d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * maxDistanceDelta / m);
}

Vector2d Vector2d::Normalized(Vector2d v)
{
    float mag = Magnitude(v);
    if (mag == 0)
        return Vector2d::Zero();
    return v / mag;
}

void Vector2d::OrthoNormalize(Vector2d &normal, Vector2d &tangent)
{
    normal = Normalized(normal);
    tangent = Reject(tangent, normal);
    tangent = Normalized(tangent);
}

Vector2d Vector2d::Project(Vector2d a, Vector2d b)
{
    float m = Magnitude(b);
    return Dot(a, b) / (m * m) * b;
}

Vector2d Vector2d::Reflect(Vector2d vector, Vector2d planeNormal)
{
    return vector - 2 * Project(vector, planeNormal);
}

Vector2d Vector2d::Reject(Vector2d a, Vector2d b)
{
    return a - Project(a, b);
}

Vector2d Vector2d::RotateTowards(Vector2d current, Vector2d target,
                               float maxRadiansDelta,
                               float maxMagnitudeDelta)
{
    float magCur = Magnitude(current);
    float magTar = Magnitude(target);
    float newMag = magCur + maxMagnitudeDelta *
        ((magTar > magCur) - (magCur > magTar));
    newMag = fmin(newMag, fmax(magCur, magTar));
    newMag = fmax(newMag, fmin(magCur, magTar));

    float totalAngle = Angle(current, target) - maxRadiansDelta;
    if (totalAngle <= 0)
        return Normalized(target) * newMag;
    else if (totalAngle >= M_PI)
        return Normalized(-target) * newMag;

    float axis = current.x * target.y - current.y * target.x;
    axis = axis / fabs(axis);
    if (!(1 - fabs(axis) < 0.00001))
        axis = 1;
    current = Normalized(current);
    Vector2d newVector = current * cos(maxRadiansDelta) +
        Vector2d(-current.y, current.x) * sin(maxRadiansDelta) * axis;
    return newVector * newMag;
}

Vector2d Vector2d::Scale(Vector2d a, Vector2d b)
{
    return Vector2d(a.x * b.x, a.y * b.y);
}

Vector2d Vector2d::Slerp(Vector2d a, Vector2d b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return SlerpUnclamped(a, b, t);
}

Vector2d Vector2d::SlerpUnclamped(Vector2d a, Vector2d b, float t)
{
    float magA = Magnitude(a);
    float magB = Magnitude(b);
    a /= magA;
    b /= magB;
    float dot = Dot(a, b);
    dot = fmax(dot, -1.0);
    dot = fmin(dot, 1.0);
    float theta = acos(dot) * t;
    Vector2d relativeVec = Normalized(b - a * dot);
    Vector2d newVec = a * cos(theta) + relativeVec * sin(theta);
    return newVec * (magA + (magB - magA) * t);
}

float Vector2d::SqrMagnitude(Vector2d v)
{
    return v.x * v.x + v.y * v.y;
}

void Vector2d::ToPolar(Vector2d vector, float &rad, float &theta)
{
    rad = Magnitude(vector);
    theta = atan2(vector.y, vector.x);
}


struct Vector2d& Vector2d::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    return *this;
}

struct Vector2d& Vector2d::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    return *this;
}

struct Vector2d& Vector2d::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

struct Vector2d& Vector2d::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

struct Vector2d& Vector2d::operator+=(const Vector2d rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

struct Vector2d& Vector2d::operator-=(const Vector2d rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2d operator-(Vector2d rhs) { return rhs * -1; }
Vector2d operator+(Vector2d lhs, const float rhs) { return lhs += rhs; }
Vector2d operator-(Vector2d lhs, const float rhs) { return lhs -= rhs; }
Vector2d operator*(Vector2d lhs, const float rhs) { return lhs *= rhs; }
Vector2d operator/(Vector2d lhs, const float rhs) { return lhs /= rhs; }
Vector2d operator+(const float lhs, Vector2d rhs) { return rhs += lhs; }
Vector2d operator-(const float lhs, Vector2d rhs) { return rhs -= lhs; }
Vector2d operator*(const float lhs, Vector2d rhs) { return rhs *= lhs; }
Vector2d operator/(const float lhs, Vector2d rhs) { return rhs /= lhs; }
Vector2d operator+(Vector2d lhs, const Vector2d rhs) { return lhs += rhs; }
Vector2d operator-(Vector2d lhs, const Vector2d rhs) { return lhs -= rhs; }

bool operator==(const Vector2d lhs, const Vector2d rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Vector2d lhs, const Vector2d rhs)
{
    return !(lhs == rhs);
}