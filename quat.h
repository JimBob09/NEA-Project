#pragma once
#include "vec.h"

struct quat
{
    float w, x, y, z;

    quat() = default;
    quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    static quat identity();
    static quat fromAxisAngle(vec3 axis, float angle);

    quat operator*(const quat& rhs) const;
    static vec3 rotate(const quat& q, const vec3& v);

    float length() const;
    quat normalise() const;
    quat conjugate() const;
};