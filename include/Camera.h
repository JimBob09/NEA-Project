#pragma once
#include "Utilities.h"
#include "vec.h"
#include "quat.h"

struct Camera
{
    vec3 position{ 0.0f, 0.0f, 3.0f };
    quat orientation = quat::identity();

    const float speed = 3.0f;
    const float sensitivity = 0.1f;
    const float rollSpeed = 45.0f;

    vec3 front() const { return quat::rotate(orientation, vec3{ 0.0f, 0.0f, -1.0f }); }
    vec3 right() const { return quat::rotate(orientation, vec3{ 1.0f, 0.0f, 0.0f }); }
    vec3 up() const { return quat::rotate(orientation, vec3{ 0.0f, 1.0f, 0.0f }); }

    void rotatePitch(float deg) { applyLocalRotation(right(), deg); }
    void rotateYaw(float deg) { applyLocalRotation(up(), deg); }
    void rotateRoll(float deg) { applyLocalRotation(front(), deg); }

private:
    void applyLocalRotation(const vec3& axis, float deg)
    {
        quat delta = quat::fromAxisAngle(axis, radians(deg));
        orientation = (delta * orientation).normalise();
    }
};
