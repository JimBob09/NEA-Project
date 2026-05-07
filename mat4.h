#pragma once
#include <xmmintrin.h>
#include <cmath>
#include "vec.h"
#include "quat.h"

// Declarations
class alignas(16) mat4
{
public:
	mat4() : m_data{} {};
	constexpr mat4(std::initializer_list<float> list);

	static constexpr mat4 identity();

	static constexpr mat4 scale(float scale);
	static constexpr mat4 scale(float x, float y, float z);
	static constexpr mat4 scale(vec3 scale);

	static constexpr mat4 translate(float x, float y, float z);
	static constexpr mat4 translate(vec3 translation);

	static mat4 rotate(const quat& rotation);

	static mat4 lookAt(vec3 position, vec3 target, vec3 up);
	static constexpr mat4 ortho(float l, float r, float b, float t, float n, float f);
	static mat4 perspective(float aspect, float fov, float n, float f);

	constexpr mat4 operator* (const mat4& rhs) const;

	const constexpr float* data() const { return m_data; }

private:
	float m_data[16];

	constexpr float& at(int row, int col) { return m_data[col * 4 + row]; }
	constexpr float at(int row, int col) const { return m_data[col * 4 + row]; }
};



// Definitions
constexpr mat4::mat4(std::initializer_list<float> list)
{
	int i = 0;
	for (float v : list)
		m_data[i++] = v;
}

constexpr mat4 mat4::identity()
{
	return mat4({
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		});
}

constexpr mat4 mat4::scale(float scale)
{
	return mat4({
		scale, 0.0f,  0.0f,  0.0f,
		0.0f,  scale, 0.0f,  0.0f,
		0.0f,  0.0f,  scale, 0.0f,
		0.0f,  0.0f,  0.0f,  1.0f
		});
}

constexpr mat4 mat4::scale(float x, float y, float z)
{
	return mat4({
		x,    0.0f, 0.0f, 0.0f,
		0.0f, y,    0.0f, 0.0f,
		0.0f, 0.0f, z,    0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		});
}

constexpr mat4 mat4::scale(vec3 scale)
{
	return mat4({
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
		});
}

constexpr mat4 mat4::translate(float x, float y, float z)
{
	return mat4({
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x,    y,    z,    1.0f
		});
}

constexpr mat4 mat4::translate(vec3 translation)
{
	return mat4({
		1.0f,		   0.0f,		  0.0f,			 0.0f,
		0.0f,		   1.0f,		  0.0f,			 0.0f,
		0.0f,          0.0f,		  1.0f,			 0.0f,
		translation.x, translation.y, translation.z, 1.0f
		});
}

inline mat4 mat4::rotate(const quat& rotation)
{
	quat q = rotation.normalise();
	return mat4({
		1 - 2 * (q.y * q.y + q.z * q.z), 2 * (q.x * q.y + q.w * q.z),     2 * (q.x * q.z - q.w * q.y),     0.0f,
		2 * (q.x * q.y - q.w * q.z),     1 - 2 * (q.x * q.x + q.z * q.z), 2 * (q.y * q.z + q.w * q.x),     0.0f,
		2 * (q.x * q.z + q.w * q.y),     2 * (q.y * q.z - q.w * q.x),     1 - 2 * (q.x * q.x + q.y * q.y), 0.0f,
		0.0f,							 0.0f,							  0.0f,							   1.0f
		});
}

inline mat4 mat4::lookAt(vec3 position, vec3 target, vec3 worldUp)
{
	vec3 direction = (position - target).normalise();
	vec3 right = cross(worldUp, direction).normalise();
	vec3 up = cross(direction, right);
	return mat4({
		right.x,                     up.x,                     direction.x,                     0.0f,
		right.y,                     up.y,                     direction.y,                     0.0f,
		right.z,                     up.z,                     direction.z,                     0.0f,
		-vec3::dot(right, position), -vec3::dot(up, position), -vec3::dot(direction, position), 1.0f
		});
}

constexpr mat4 mat4::ortho(float l, float r, float b, float t, float n, float f)
{
	return mat4({
		2.0f / (r - l),     0.0f,               0.0f,               0.0f,
		0.0f,               2.0f / (t - b),     0.0f,               0.0f,
		0.0f,               0.0f,               -2.0f / (f - n),    0.0f,
		-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1.0f
		});
}

inline mat4 mat4::perspective(float aspect, float fov, float n, float f)
{
	return mat4({
		1.0f / (aspect * tanf(fov * 0.5f)), 0.0f,                    0.0f,						0.0f,
		0.0f,                               1.0f / tanf(fov * 0.5f), 0.0f,						0.0f,
		0.0f,                               0.0f,                    (f + n) / (n - f),			-1.0f,
		0.0f,                               0.0f,                    (2.0f * f * n) / (n - f) , 0.0f
		});
}

constexpr mat4 mat4::operator* (const mat4& rhs) const
{
	if consteval {
		mat4 result;

		for (int col = 0; col < 4; col++)
		{
			for (int row = 0; row < 4; row++)
			{
				float sum = 0.0f;

				for (int i = 0; i < 4; i++)
					sum += at(row, i) * rhs.at(i, col);

				result.at(row, col) = sum;
			}
		}

		return result;
	}
	else {
		mat4 result;

		// Load the columns of lhs into registers
		__m128 lhs_col0 = _mm_load_ps(&m_data[0]);
		__m128 lhs_col1 = _mm_load_ps(&m_data[4]);
		__m128 lhs_col2 = _mm_load_ps(&m_data[8]);
		__m128 lhs_col3 = _mm_load_ps(&m_data[12]);

		for (int i = 0; i < 4; i++) {
			// Broadcast the each of the elements of the ith column of rhs into their own registers
			__m128 rhs_comp0 = _mm_set1_ps(rhs.m_data[i * 4]);
			__m128 rhs_comp1 = _mm_set1_ps(rhs.m_data[i * 4 + 1]);
			__m128 rhs_comp2 = _mm_set1_ps(rhs.m_data[i * 4 + 2]);
			__m128 rhs_comp3 = _mm_set1_ps(rhs.m_data[i * 4 + 3]);

			// Multiply each of the elements of the ith column of rhs with the columns of lhs
			__m128 m0 = _mm_mul_ps(lhs_col0, rhs_comp0);
			__m128 m1 = _mm_mul_ps(lhs_col1, rhs_comp1);
			__m128 m2 = _mm_mul_ps(lhs_col2, rhs_comp2);
			__m128 m3 = _mm_mul_ps(lhs_col3, rhs_comp3);

			// Add all of these multiplications together to get each collumn of the result
			__m128 res_col = _mm_add_ps(_mm_add_ps(m0, m1), _mm_add_ps(m2, m3));

			// Store that column into the result matrix
			_mm_store_ps(&result.m_data[i * 4], res_col);
		}

		return result;
	}
}
