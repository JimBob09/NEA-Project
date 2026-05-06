#pragma once
#include <initializer_list>
#include <cmath>
#include <algorithm>

// Specialisations
template<typename T, int N>
struct vec_base
{
    T data[N];
};

template<typename T>
struct vec_base<T, 2>
{
    union { T data[2]; struct { T x, y; }; };
};

template<typename T>
struct vec_base<T, 3>
{
    union { T data[3]; struct { T x, y, z; }; };
};

template<typename T>
struct vec_base<T, 4>
{
    union { T data[4]; struct { T x, y, z, w; }; };
};

// Declarations
template<typename T, int N>
struct vec : vec_base<T, N>
{
    vec() = default;
    vec(std::initializer_list<T> list);

    T length() const;
    vec<T, N> normalise() const;

    static T dot(const vec<T, N>& lhs, const vec<T, N>& rhs);

    T& operator[](int i);
    const T& operator[](int i) const;

    vec<T, N> operator+(const vec<T, N>& rhs) const;
    vec<T, N>& operator+=(const vec<T, N>& rhs);

    vec<T, N> operator-(const vec<T, N>& rhs) const;
    vec<T, N>& operator-=(const vec<T, N>& rhs);
    vec<T, N> operator-() const;

    vec<T, N> operator*(T scalar) const;
    vec<T, N> operator/(T scalar) const;
};

template<typename T, int N>
vec<T, N> operator*(T scalar, const vec<T, N>& vec);

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;

inline vec3 cross(const vec3& lhs, const vec3& rhs);



// Definitions
template<typename T, int N>
vec<T, N>::vec(std::initializer_list<T> list)
{
    int i = 0;
    for (T v : list)
        this->data[i++] = v;
}

template<typename T, int N>
T vec<T, N>::length() const
{
    T result = 0;
    for (int i = 0; i < N; i++)
        result += this->data[i] * this->data[i];
    return static_cast<T>(std::sqrt(result));
}

template<typename T, int N>
vec<T, N> vec<T, N>::normalise() const
{
    return (*this) / length();
}

template<typename T, int N>
T vec<T, N>::dot(const vec<T, N>& lhs, const vec<T, N>& rhs)
{
    T result = 0;
    for (int i = 0; i < N; i++)
        result += lhs[i] * rhs[i];
    return result;
}

template<typename T, int N>
T& vec<T, N>::operator[](int i)
{
    return this->data[i];
}

template<typename T, int N>
const T& vec<T, N>::operator[](int i) const
{
    return this->data[i];
}

template<typename T, int N>
vec<T, N> vec<T, N>::operator+(const vec<T, N>& rhs) const
{
    vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = this->data[i] + rhs[i];
    return result;
}

template<typename T, int N>
vec<T, N>& vec<T, N>::operator+=(const vec<T, N>& rhs)
{
    for (int i = 0; i < N; i++)
        this->data[i] += rhs[i];
    return *this;
}

template<typename T, int N>
vec<T, N> vec<T, N>::operator-(const vec<T, N>& rhs) const
{
    vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = this->data[i] - rhs[i];
    return result;
}

template<typename T, int N>
vec<T, N>& vec<T, N>::operator-=(const vec<T, N>& rhs)
{
    for (int i = 0; i < N; i++)
        this->data[i] -= rhs[i];
    return *this;
}

template<typename T, int N>
vec<T, N> vec<T, N>::operator-() const
{
    vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = -this->data[i];
    return result;
}

template<typename T, int N>
vec<T, N> vec<T, N>::operator*(T scalar) const
{
    vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = this->data[i] * scalar;
    return result;
}

template<typename T, int N>
vec<T, N> vec<T, N>::operator/(T scalar) const
{
    return (*this) * (static_cast<T>(1) / scalar);
}

template<typename T, int N>
vec<T, N> operator*(T scalar, const vec<T, N>& vec)
{
    return vec * scalar;
}

inline vec3 cross(const vec3& lhs, const vec3& rhs)
{
    return vec3{
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    };
}
