#pragma once
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

inline float radians(float degrees)
{
    return degrees * (static_cast<float>(M_PI) / 180.0f);
}