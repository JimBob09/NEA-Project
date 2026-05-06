#include <cmath>
#include "vec.h"
#include "quat.h"

quat quat::identity()
{
    return quat(1.0f, 0.0f, 0.0f, 0.0f);
}

quat quat::fromAxisAngle(vec3 axis, float angle)
{
	float halfAngle = angle / 2;
	float sinAngle = sinf(halfAngle);
	return quat(cosf(halfAngle), axis.x * sinAngle, axis.y * sinAngle, axis.z * sinAngle);
}

quat quat::operator*(const quat& rhs) const
{
    return quat(
        w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
        w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
        w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
        w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
    );
}

vec3 quat::rotate(const quat& q, const vec3& v)
{
    quat p(0.0f, v.x, v.y, v.z);
    quat result = q * p * q.conjugate();
    return vec3{ result.x, result.y, result.z };
}

float quat::length() const
{
    return sqrtf(w * w + x * x + y * y + z * z);
}

quat quat::normalise() const
{
    float len = length();
    return quat(w / len, x / len, y / len, z / len);
}

quat quat::conjugate() const
{
    return quat(w, -x, -y, -z);
}
