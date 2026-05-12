#pragma once
#include <iostream>
#include <numbers>

constexpr float radians(float degrees)
{
    return degrees * (std::numbers::pi_v<float> / 180.0f);
}