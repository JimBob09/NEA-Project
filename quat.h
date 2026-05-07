#pragma once
#include <cmath>
#include "vec.h"

// Declarations
struct quat
{
    float w, x, y, z;

    quat() = default;
    constexpr quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    static constexpr quat identity();
    static quat fromAxisAngle(vec3 axis, float angle);

    constexpr quat operator*(const quat& rhs) const;
    static constexpr vec3 rotate(const quat& q, const vec3& v);

    float length() const;
    quat normalise() const;
    constexpr quat conjugate() const;
};



// Definitions
constexpr quat quat::identity()
{
    return quat(1.0f, 0.0f, 0.0f, 0.0f);
}

inline quat quat::fromAxisAngle(vec3 axis, float angle)
{
    float halfAngle = angle / 2;
    float sinAngle = sinf(halfAngle);
    vec3 nAxis = axis.normalise();
    return quat(cosf(halfAngle), nAxis.x * sinAngle, nAxis.y * sinAngle, nAxis.z * sinAngle);
}

constexpr quat quat::operator*(const quat& rhs) const
{
    return quat(
        w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
        w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
        w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
        w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
    );
}

constexpr vec3 quat::rotate(const quat& q, const vec3& v)
{
    quat p(0.0f, v.x, v.y, v.z);
    quat result = q * p * q.conjugate();
    return vec3{ result.x, result.y, result.z };
}

inline float quat::length() const
{
    return sqrtf(w * w + x * x + y * y + z * z);
}

inline quat quat::normalise() const
{
    float len = length();
    return quat(w / len, x / len, y / len, z / len);
}

constexpr quat quat::conjugate() const
{
    return quat(w, -x, -y, -z);
}
