#pragma once
#include <immintrin.h>
#include <cmath>
#include "vec.h"

// Declarations
struct alignas(16) quat
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
    float s = sinf(halfAngle);
    float c = cosf(halfAngle);

    vec3 nAxis = axis.normalise();

    // Load values into registers
    __m128 a = _mm_setr_ps(1.0f, nAxis.x, nAxis.y, nAxis.z);
    __m128 trig = _mm_setr_ps(c, s, s, s);

    // Multiply the axis with the trig values
    __m128 res = _mm_mul_ps(a, trig);

    // Store result back into a quaternion
    quat result;
    _mm_store_ps(&result.w, res);
    return result;
}

constexpr quat quat::operator*(const quat& rhs) const
{
    if consteval {
        return quat(
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
            w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
        );
    }
    else {
        // Broadcast components of lhs
        __m128 lhs_wwww = _mm_set1_ps(w);
        __m128 lhs_xxxx = _mm_set1_ps(x);
        __m128 lhs_yyyy = _mm_set1_ps(y);
        __m128 lhs_zzzz = _mm_set1_ps(z);

        // Shuffle components of rhs
        __m128 rhs_wxyz = _mm_load_ps(&rhs.w);
        __m128 rhs_xwzy = _mm_shuffle_ps(rhs_wxyz, rhs_wxyz, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 rhs_yzwx = _mm_shuffle_ps(rhs_wxyz, rhs_wxyz, _MM_SHUFFLE(1, 0, 3, 2));
        __m128 rhs_zyxw = _mm_shuffle_ps(rhs_wxyz, rhs_wxyz, _MM_SHUFFLE(0, 1, 2, 3));

        // Multiply broadcasts of lhs with shuffles of rhs
        __m128 m0 = _mm_mul_ps(lhs_wwww, rhs_wxyz);
        __m128 m1 = _mm_mul_ps(lhs_xxxx, rhs_xwzy);
        __m128 m2 = _mm_mul_ps(lhs_yyyy, rhs_yzwx);
        __m128 m3 = _mm_mul_ps(lhs_zzzz, rhs_zyxw);

        // m0 + m1 -> { -, +, -, + }
        __m128 sum01 = _mm_addsub_ps(m0, m1);

        // m2 -> { -, +, +, - }
        __m128 mask2 = _mm_setr_ps(-0.0f, 0.0f, 0.0f, -0.0f);
        m2 = _mm_xor_ps(m2, mask2);

        // m3 -> { -, -, +, + }
        __m128 mask3 = _mm_setr_ps(-0.0f, -0.0f, 0.0f, 0.0f);
        m3 = _mm_xor_ps(m3, mask3);

        // Sum everything together
        __m128 res = _mm_add_ps(sum01, _mm_add_ps(m2, m3));

        // Store result back into a quaternion
        quat result;
        _mm_store_ps(&result.w, res);
        return result;
    }
}

constexpr vec3 quat::rotate(const quat& q, const vec3& v)
{
    if consteval {
        quat p(0.0f, v.x, v.y, v.z);
        quat result = q * p * q.conjugate();
        return vec3{ result.x, result.y, result.z };
    }
    else {
        // Load quaternion and vector into registers
        __m128 q_wxyz = _mm_load_ps(&q.w);
        __m128 v_xyz = _mm_setr_ps(0.0f, v.x, v.y, v.z);

        // Broadcast w component of q into a register 
        __m128 q_wwww = _mm_shuffle_ps(q_wxyz, q_wxyz, _MM_SHUFFLE(0, 0, 0, 0));

        // Lambda for cross product
        auto cross = [](__m128 a, __m128 b) {
            // Shuffle components of a and z
            __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 3, 2, 0));
            __m128 a_zxy = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 1, 3, 0));
            __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 3, 2, 0));
            __m128 b_zxy = _mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 1, 3, 0));

            // Multiply and subtract the shuffled vectors to get cross product
            return _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx));
        };

        // Vector rotation: v' = v + 2q x (q x v + q_w * v)
        __m128 res = _mm_add_ps(v_xyz, cross(_mm_add_ps(q_wxyz, q_wxyz), _mm_add_ps(cross(q_wxyz, v_xyz), _mm_mul_ps(q_wwww, v_xyz))));

        // Temporarily store in array as vector is not aligned and then return last three components as vector
        alignas(16) float f[4];
        _mm_store_ps(f, res);
        return vec3{ f[1], f[2], f[3] };
    }
}

inline float quat::length() const
{
    // Load quaternion into a register
    __m128 q = _mm_load_ps(&w);

    // Broadcasts the dot product between r and itself into a register
    __m128 dot = _mm_dp_ps(q, q, 0xFF);

    // Square roots the first value and returns as float
    return _mm_cvtss_f32(_mm_sqrt_ss(dot));
}

inline quat quat::normalise() const
{
    // Load quaternion into a register
    __m128 q = _mm_load_ps(&w);

    // Broadcasts the dot product between r and itself into a register
    __m128 dot = _mm_dp_ps(q, q, 0xFF);

    // Stores the reciprocal square root approximation into a reister
    __m128 rsqrt = _mm_rsqrt_ps(dot);

    // Broadcast numbers into registers
    __m128 half = _mm_set1_ps(0.5f);
    __m128 three_halves = _mm_set1_ps(1.5f);

    // Newton-Raphson iteration: y = y * (1.5 - 0.5 * x * y * y)
    __m128 res = _mm_mul_ps(rsqrt, _mm_sub_ps(three_halves, _mm_mul_ps(_mm_mul_ps(half, dot), _mm_mul_ps(rsqrt, rsqrt))));

    // Multiply with original quaternion register and store back into quaternion
    quat result;
    _mm_store_ps(&result.w, _mm_mul_ps(q, res));
    return result;
}

constexpr quat quat::conjugate() const
{
    if consteval {
        return quat(w, -x, -y, -z);
    }
    else {
        // Load quaternion into a register
        __m128 q = _mm_load_ps(&w);

        // r -> { +, -, -, - }
        __m128 mask = _mm_setr_ps(0.0f, -0.0f, -0.0f, -0.0f);
        q = _mm_xor_ps(q, mask);

        // Store back into quaternion
        quat result;
        _mm_store_ps(&result.w, q);
        return result;
    }
}
