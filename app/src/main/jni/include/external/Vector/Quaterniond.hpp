#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define SMALL_float 0.0000000001

#include "Vector3d.hpp"

struct Quaterniond
{
    union
    {
        struct
        {
            double x;
            double y;
            double z;
            double w;
        };
        double data[4];
    };


    /**
     * Constructors.
     */
    inline Quaterniond();
    inline Quaterniond(double data[]);
    inline Quaterniond(Vector3d vector, double scalar);
    inline Quaterniond(double x, double y, double z, double w);


    /**
     * Constants for common quaternions.
     */
    static inline Quaterniond Identity();


    /**
     * Returns the angle between two quaternions.
     * The quaternions must be normalized.
     * @param a: The first quaternion.
     * @param b: The second quaternion.
     * @return: A scalar value.
     */
    static inline float Angle(Quaterniond a, Quaterniond b);

    /**
     * Returns the conjugate of a quaternion.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static inline Quaterniond Conjugate(Quaterniond rotation);

    /**
     * Returns the dot product of two quaternions.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Quaterniond lhs, Quaterniond rhs);

    /**
     * Creates a new quaternion from the angle-axis representation of
     * a rotation.
     * @param angle: The rotation angle in radians.
     * @param axis: The vector about which the rotation occurs.
     * @return: A new quaternion.
     */
    static inline Quaterniond FromAngleAxis(float angle, Vector3d axis);

    /**
     * Create a new quaternion from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param rotation: The x, y and z rotations.
     * @return: A new quaternion.
     */
    static inline Quaterniond FromEuler(Vector3d rotation);

    /**
     * Create a new quaternion from the euler angle representation of
     * a rotation. The z, x and y values represent rotations about those
     * axis in that respective order.
     * @param x: The rotation about the x-axis in radians.
     * @param y: The rotation about the y-axis in radians.
     * @param z: The rotation about the z-axis in radians.
     * @return: A new quaternion.
     */
    static inline Quaterniond FromEuler(float x, float y, float z);

    /**
     * Create a quaternion rotation which rotates "fromVector" to "toVector".
     * @param fromVector: The vector from which to start the rotation.
     * @param toVector: The vector at which to end the rotation.
     * @return: A new quaternion.
     */
    static inline Quaterniond FromToRotation(Vector3d fromVector,
                                            Vector3d toVector);

    /**
     * Returns the inverse of a rotation.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static inline Quaterniond Inverse(Quaterniond rotation);

    /**
     * Interpolates between a and b by t, which is clamped to the range [0-1].
     * The result is normalized before being returned.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @return: A new quaternion.
     */
    static inline Quaterniond Lerp(Quaterniond a, Quaterniond b, float t);

    /**
     * Interpolates between a and b by t. This normalizes the result when
     * complete.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static inline Quaterniond LerpUnclamped(Quaterniond a, Quaterniond b,
                                           float t);

    /**
     * Creates a rotation with the specified forward direction. This is the
     * same as calling LookRotation with (0, 1, 0) as the upwards vector.
     * The output is undefined for parallel vectors.
     * @param forward: The forward direction to look toward.
     * @return: A new quaternion.
     */
    static inline Quaterniond LookRotation(Vector3d forward);

    /**
     * Creates a rotation with the specified forward and upwards directions.
     * The output is undefined for parallel vectors.
     * @param forward: The forward direction to look toward.
     * @param upwards: The direction to treat as up.
     * @return: A new quaternion.
     */
    static inline Quaterniond LookRotation(Vector3d forward, Vector3d upwards);

    /**
     * Returns the norm of a quaternion.
     * @param rotation: The quaternion in question.
     * @return: A scalar value.
     */
    static inline float Norm(Quaterniond rotation);

    /**
     * Returns a quaternion with identical rotation and a norm of one.
     * @param rotation: The quaternion in question.
     * @return: A new quaternion.
     */
    static inline Quaterniond Normalized(Quaterniond rotation);

    /**
     * Returns a new Quaterniond created by rotating "from" towards "to" by
     * "maxRadiansDelta". This will not overshoot, and if a negative delta is
     * applied, it will rotate till completely opposite "to" and then stop.
     * @param from: The rotation at which to start.
     * @param to: The rotation at which to end.
     # @param maxRadiansDelta: The maximum number of radians to rotate.
     * @return: A new Quaterniond.
     */
    static inline Quaterniond RotateTowards(Quaterniond from, Quaterniond to,
                                           float maxRadiansDelta);

    /**
     * Returns a new quaternion interpolated between a and b, using spherical
     * linear interpolation. The variable t is clamped to the range [0-1]. The
     * resulting quaternion will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static inline Quaterniond Slerp(Quaterniond a, Quaterniond b, float t);

    /**
     * Returns a new quaternion interpolated between a and b, using spherical
     * linear interpolation. The resulting quaternion will be normalized.
     * @param a: The starting rotation.
     * @param b: The ending rotation.
     * @param t: The interpolation value.
     * @return: A new quaternion.
     */
    static inline Quaterniond SlerpUnclamped(Quaterniond a, Quaterniond b,
                                            float t);

    /**
     * Outputs the angle axis representation of the provided quaternion.
     * @param rotation: The input quaternion.
     * @param angle: The output angle.
     * @param axis: The output axis.
     */
    static inline void ToAngleAxis(Quaterniond rotation, float &angle,
                                   Vector3d &axis);

    /**
     * Returns the Euler angle representation of a rotation. The resulting
     * vector contains the rotations about the z, x and y axis, in that order.
     * @param rotation: The quaternion to convert.
     * @return: A new vector.
     */
    static inline Vector3d ToEuler(Quaterniond rotation);

        std::string to_string() { 
        char buf[128];
        // float বা double যাই হোক, %.2f দিয়ে ২ দশমিক ঘর পর্যন্ত ফরম্যাট করা যাবে
        snprintf(buf, sizeof(buf), "%.2f, %.2f, %.2f, %.2f", x, y, z, w);
        return std::string(buf);
    }
    std::string ToString() { return to_string(); }

    explicit operator bool () const { return x != 0 || y != 0 || z != 0 || w != 0; }

    /**
     * Operator overloading.
     */
    inline struct Quaterniond& operator+=(const float rhs);
    inline struct Quaterniond& operator-=(const float rhs);
    inline struct Quaterniond& operator*=(const float rhs);
    inline struct Quaterniond& operator/=(const float rhs);
    inline struct Quaterniond& operator+=(const Quaterniond rhs);
    inline struct Quaterniond& operator-=(const Quaterniond rhs);
    inline struct Quaterniond& operator*=(const Quaterniond rhs);
};

inline Quaterniond operator-(Quaterniond rhs);
inline Quaterniond operator+(Quaterniond lhs, const float rhs);
inline Quaterniond operator-(Quaterniond lhs, const float rhs);
inline Quaterniond operator*(Quaterniond lhs, const float rhs);
inline Quaterniond operator/(Quaterniond lhs, const float rhs);
inline Quaterniond operator+(const float lhs, Quaterniond rhs);
inline Quaterniond operator-(const float lhs, Quaterniond rhs);
inline Quaterniond operator*(const float lhs, Quaterniond rhs);
inline Quaterniond operator/(const float lhs, Quaterniond rhs);
inline Quaterniond operator+(Quaterniond lhs, const Quaterniond rhs);
inline Quaterniond operator-(Quaterniond lhs, const Quaterniond rhs);
inline Quaterniond operator*(Quaterniond lhs, const Quaterniond rhs);
inline Vector3d operator*(Quaterniond lhs, const Vector3d rhs);
inline bool operator==(const Quaterniond lhs, const Quaterniond rhs);
inline bool operator!=(const Quaterniond lhs, const Quaterniond rhs);



/*******************************************************************************
 * Implementation
 */

Quaterniond::Quaterniond() : x(0), y(0), z(0), w(1) {}
Quaterniond::Quaterniond(double data[]) : x(data[0]), y(data[1]), z(data[2]),
                                       w(data[3]) {}
Quaterniond::Quaterniond(Vector3d vector, double scalar) : x(vector.x),
                                                       y(vector.y), z(vector.z), w(scalar) {}
Quaterniond::Quaterniond(double x, double y, double z, double w) : x(x), y(y),
                                                             z(z), w(w) {}


Quaterniond Quaterniond::Identity() { return Quaterniond(0, 0, 0, 1); }


float Quaterniond::Angle(Quaterniond a, Quaterniond b)
{
    float dot = Dot(a, b);
    return acos(fmin(fabs(dot), 1)) * 2;
}

Quaterniond Quaterniond::Conjugate(Quaterniond rotation)
{
    return Quaterniond(-rotation.x, -rotation.y, -rotation.z, rotation.w);
}

float Quaterniond::Dot(Quaterniond lhs, Quaterniond rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

Quaterniond Quaterniond::FromAngleAxis(float angle, Vector3d axis)
{
    Quaterniond q;
    float m = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    float s = sin(angle / 2) / m;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = cos(angle / 2);
    return q;
}

Quaterniond Quaterniond::FromEuler(Vector3d rotation)
{
    return FromEuler(rotation.x, rotation.y, rotation.z);
}

Quaterniond Quaterniond::FromEuler(float x, float y, float z)
{
    float cx = cos(x * 0.5);
    float cy = cos(y * 0.5);
    float cz = cos(z * 0.5);
    float sx = sin(x * 0.5);
    float sy = sin(y * 0.5);
    float sz = sin(z * 0.5);
    Quaterniond q;
    q.x = cx * sy * sz + cy * cz * sx;
    q.y = cx * cz * sy - cy * sx * sz;
    q.z = cx * cy * sz - cz * sx * sy;
    q.w = sx * sy * sz + cx * cy * cz;
    return q;
}

Quaterniond Quaterniond::FromToRotation(Vector3d fromVector, Vector3d toVector)
{
    float dot = Vector3d::Dot(fromVector, toVector);
    float k = sqrt(Vector3d::SqrMagnitude(fromVector) *
                   Vector3d::SqrMagnitude(toVector));
    if (fabs(dot / k + 1) < 0.00001)
    {
        Vector3d ortho = Vector3d::Orthogonal(fromVector);
        return Quaterniond(Vector3d::Normalized(ortho), 0);
    }
    Vector3d cross = Vector3d::Cross(fromVector, toVector);
    return Normalized(Quaterniond(cross, dot + k));
}

Quaterniond Quaterniond::Inverse(Quaterniond rotation)
{
    float n = Norm(rotation);
    return Conjugate(rotation) / (n * n);
}

Quaterniond Quaterniond::Lerp(Quaterniond a, Quaterniond b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return LerpUnclamped(a, b, t);
}

Quaterniond Quaterniond::LerpUnclamped(Quaterniond a, Quaterniond b, float t)
{
    Quaterniond quaternion;
    if (Dot(a, b) >= 0)
        quaternion = a * (1 - t) + b * t;
    else
        quaternion = a * (1 - t) - b * t;
    return Normalized(quaternion);
}

Quaterniond Quaterniond::LookRotation(Vector3d forward)
{
    return LookRotation(forward, Vector3d(0, 1, 0));
}

Quaterniond Quaterniond::LookRotation(Vector3d forward, Vector3d upwards)
{
    // Normalize inputs
    forward = Vector3d::Normalized(forward);
    upwards = Vector3d::Normalized(upwards);
    // Don't allow zero vectors
    if (Vector3d::SqrMagnitude(forward) < SMALL_float || Vector3d::SqrMagnitude(upwards) < SMALL_float)
        return Quaterniond::Identity();
    // Handle alignment with up direction
    if (1 - fabs(Vector3d::Dot(forward, upwards)) < SMALL_float)
        return FromToRotation(Vector3d::Forward(), forward);
    // Get orthogonal vectors
    Vector3d right = Vector3d::Normalized(Vector3d::Cross(upwards, forward));
    upwards = Vector3d::Cross(forward, right);
    // Calculate rotation
    Quaterniond quaternion;
    float radicand = right.x + upwards.y + forward.z;
    if (radicand > 0)
    {
        quaternion.w = sqrt(1.0 + radicand) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.w);
        quaternion.x = (upwards.z - forward.y) * recip;
        quaternion.y = (forward.x - right.z) * recip;
        quaternion.z = (right.y - upwards.x) * recip;
    }
    else if (right.x >= upwards.y && right.x >= forward.z)
    {
        quaternion.x = sqrt(1.0 + right.x - upwards.y - forward.z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.x);
        quaternion.w = (upwards.z - forward.y) * recip;
        quaternion.z = (forward.x + right.z) * recip;
        quaternion.y = (right.y + upwards.x) * recip;
    }
    else if (upwards.y > forward.z)
    {
        quaternion.y = sqrt(1.0 - right.x + upwards.y - forward.z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.y);
        quaternion.z = (upwards.z + forward.y) * recip;
        quaternion.w = (forward.x - right.z) * recip;
        quaternion.x = (right.y + upwards.x) * recip;
    }
    else
    {
        quaternion.z = sqrt(1.0 - right.x - upwards.y + forward.z) * 0.5;
        float recip = 1.0 / (4.0 * quaternion.z);
        quaternion.y = (upwards.z + forward.y) * recip;
        quaternion.x = (forward.x + right.z) * recip;
        quaternion.w = (right.y - upwards.x) * recip;
    }
    return quaternion;
}

float Quaterniond::Norm(Quaterniond rotation)
{
    return sqrt(rotation.x * rotation.x +
                rotation.y * rotation.y +
                rotation.z * rotation.z +
                rotation.w * rotation.w);
}

Quaterniond Quaterniond::Normalized(Quaterniond rotation)
{
    return rotation / Norm(rotation);
}

Quaterniond Quaterniond::RotateTowards(Quaterniond from, Quaterniond to,
                                     float maxRadiansDelta)
{
    float angle = Quaterniond::Angle(from, to);
    if (angle == 0)
        return to;
    maxRadiansDelta = fmax(maxRadiansDelta, angle - M_PI);
    float t = fmin(1, maxRadiansDelta / angle);
    return Quaterniond::SlerpUnclamped(from, to, t);
}

Quaterniond Quaterniond::Slerp(Quaterniond a, Quaterniond b, float t)
{
    if (t < 0) return Normalized(a);
    else if (t > 1) return Normalized(b);
    return SlerpUnclamped(a, b, t);
}

Quaterniond Quaterniond::SlerpUnclamped(Quaterniond a, Quaterniond b, float t)
{
    float n1;
    float n2;
    float n3 = Dot(a, b);
    bool flag = false;
    if (n3 < 0)
    {
        flag = true;
        n3 = -n3;
    }
    if (n3 > 0.999999)
    {
        n2 = 1 - t;
        n1 = flag ? -t : t;
    }
    else
    {
        float n4 = acos(n3);
        float n5 = 1 / sin(n4);
        n2 = sin((1 - t) * n4) * n5;
        n1 = flag ? -sin(t * n4) * n5 : sin(t * n4) * n5;
    }
    Quaterniond quaternion;
    quaternion.x = (n2 * a.x) + (n1 * b.x);
    quaternion.y = (n2 * a.y) + (n1 * b.y);
    quaternion.z = (n2 * a.z) + (n1 * b.z);
    quaternion.w = (n2 * a.w) + (n1 * b.w);
    return Normalized(quaternion);
}

void Quaterniond::ToAngleAxis(Quaterniond rotation, float &angle, Vector3d &axis)
{
    if (rotation.w > 1)
        rotation = Normalized(rotation);
    angle = 2 * acos(rotation.w);
    float s = sqrt(1 - rotation.w * rotation.w);
    if (s < 0.00001) {
        axis.x = 1;
        axis.y = 0;
        axis.z = 0;
    } else {
        axis.x = rotation.x / s;
        axis.y = rotation.y / s;
        axis.z = rotation.z / s;
    }
}

Vector3d Quaterniond::ToEuler(Quaterniond rotation)
{
    float sqw = rotation.w * rotation.w;
    float sqx = rotation.x * rotation.x;
    float sqy = rotation.y * rotation.y;
    float sqz = rotation.z * rotation.z;
    // If normalized is one, otherwise is correction factor
    float unit = sqx + sqy + sqz + sqw;
    float test = rotation.x * rotation.w - rotation.y * rotation.z;
    Vector3d v;
    // Singularity at north pole
    if (test > 0.4995f * unit)
    {
        v.y = 2 * atan2(rotation.y, rotation.x);
        v.x = M_PI_2;
        v.z = 0;
        return v;
    }
    // Singularity at south pole
    if (test < -0.4995f * unit)
    {
        v.y = -2 * atan2(rotation.y, rotation.x);
        v.x = -M_PI_2;
        v.z = 0;
        return v;
    }
    // Yaw
    v.y = atan2(2 * rotation.w * rotation.y + 2 * rotation.z * rotation.x,
                1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y));
    // Pitch
    v.x = asin(2 * (rotation.w * rotation.x - rotation.y * rotation.z));
    // Roll
    v.z = atan2(2 * rotation.w * rotation.z + 2 * rotation.x * rotation.y,
                1 - 2 * (rotation.z * rotation.z + rotation.x * rotation.x));
    return v;
}

struct Quaterniond& Quaterniond::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;
    w += rhs;
    return *this;
}

struct Quaterniond& Quaterniond::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    w -= rhs;
    return *this;
}

struct Quaterniond& Quaterniond::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
}

struct Quaterniond& Quaterniond::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
}

struct Quaterniond& Quaterniond::operator+=(const Quaterniond rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

struct Quaterniond& Quaterniond::operator-=(const Quaterniond rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

struct Quaterniond& Quaterniond::operator*=(const Quaterniond rhs)
{
    Quaterniond q;
    q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
    q.x = x * rhs.w + w * rhs.x + y * rhs.z - z * rhs.y;
    q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
    q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
    *this = q;
    return *this;
}

Quaterniond operator-(Quaterniond rhs) { return rhs * -1; }
Quaterniond operator+(Quaterniond lhs, const float rhs) { return lhs += rhs; }
Quaterniond operator-(Quaterniond lhs, const float rhs) { return lhs -= rhs; }
Quaterniond operator*(Quaterniond lhs, const float rhs) { return lhs *= rhs; }
Quaterniond operator/(Quaterniond lhs, const float rhs) { return lhs /= rhs; }
Quaterniond operator+(const float lhs, Quaterniond rhs) { return rhs += lhs; }
Quaterniond operator-(const float lhs, Quaterniond rhs) { return rhs -= lhs; }
Quaterniond operator*(const float lhs, Quaterniond rhs) { return rhs *= lhs; }
Quaterniond operator/(const float lhs, Quaterniond rhs) { return rhs /= lhs; }
Quaterniond operator+(Quaterniond lhs, const Quaterniond rhs)
{
    return lhs += rhs;
}
Quaterniond operator-(Quaterniond lhs, const Quaterniond rhs)
{
    return lhs -= rhs;
}
Quaterniond operator*(Quaterniond lhs, const Quaterniond rhs)
{
    return lhs *= rhs;
}

Vector3d operator*(Quaterniond lhs, const Vector3d rhs)
{
    Vector3d u = Vector3d(lhs.x, lhs.y, lhs.z);
    float s = lhs.w;
    return u * (Vector3d::Dot(u, rhs) * 2)
           + rhs * (s * s - Vector3d::Dot(u, u))
           + Vector3d::Cross(u, rhs) * (2.0 * s);
}

bool operator==(const Quaterniond lhs, const Quaterniond rhs)
{
    return lhs.x == rhs.x &&
           lhs.y == rhs.y &&
           lhs.z == rhs.z &&
           lhs.w == rhs.w;
}

bool operator!=(const Quaterniond lhs, const Quaterniond rhs)
{
    return !(lhs == rhs);
}