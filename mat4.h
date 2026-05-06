#pragma once
#include "vec.h"
#include "quat.h"

class mat4
{
public:
	mat4() : m_data{} {};
	mat4(std::initializer_list<float> list);

	static mat4 identity();

	static mat4 scale(float scale);
	static mat4 scale(float x, float y, float z);
	static mat4 scale(vec3 scale);

	static mat4 translate(float x, float y, float z);
	static mat4 translate(vec3 translation);

	static mat4 rotate(const quat& rotation);

	static mat4 lookAt(vec3 position, vec3 target, vec3 up);
	static mat4 ortho(float l, float r, float b, float t, float n, float f);
	static mat4 perspective(float aspect, float fov, float n, float f);

	mat4 operator* (const mat4& rhs) const;

	const float* ptr() const { return m_data; }

private:
	float m_data[16];

	float& at(int row, int col) { return m_data[col * 4 + row]; }
	float at(int row, int col) const { return m_data[col * 4 + row]; }
};
