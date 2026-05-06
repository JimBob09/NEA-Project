#include <cmath>
#include "mat4.h"
#include "vec.h"

mat4::mat4(std::initializer_list<float> list)
{
	int i = 0;
	for (float v : list)
		m_data[i++] = v;
}

mat4 mat4::identity()
{
	return mat4({ 
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	});
}

mat4 mat4::scale(float scale)
{
	return mat4({
		scale, 0.0f,  0.0f,  0.0f,
		0.0f,  scale, 0.0f,  0.0f,
		0.0f,  0.0f,  scale, 0.0f,
		0.0f,  0.0f,  0.0f,  1.0f
	});
}

mat4 mat4::scale(float x, float y, float z)
{
	return mat4({ 
		x,    0.0f, 0.0f, 0.0f,
		0.0f, y,    0.0f, 0.0f,
	    0.0f, 0.0f, z,    0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	});
}

mat4 mat4::scale(vec3 scale)
{
	return mat4({
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	});
}

mat4 mat4::translate(float x, float y, float z)
{
	return mat4({
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x,    y,    z,    1.0f
	});
}

mat4 mat4::translate(vec3 translation)
{
	return mat4({
		1.0f,		   0.0f,		  0.0f,			 0.0f,
		0.0f,		   1.0f,		  0.0f,			 0.0f,
		0.0f,          0.0f,		  1.0f,			 0.0f,
		translation.x, translation.y, translation.z, 1.0f
	});
}

mat4 mat4::rotate(const quat& rotation)
{
	quat q = rotation.normalise();
	return mat4({
		1 - 2 * (q.y * q.y + q.z * q.z), 2 * (q.x * q.y + q.w * q.z),     2 * (q.x * q.z - q.w * q.y),     0.0f,
		2 * (q.x * q.y - q.w * q.z),     1 - 2 * (q.x * q.x + q.z * q.z), 2 * (q.y * q.z + q.w * q.x),     0.0f,
		2 * (q.x * q.z + q.w * q.y),     2 * (q.y * q.z - q.w * q.x),     1 - 2 * (q.x * q.x + q.y * q.y), 0.0f,
		0.0f,							 0.0f,							  0.0f,							   1.0f
	});
}

mat4 mat4::lookAt(vec3 position, vec3 target, vec3 worldUp)
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

mat4 mat4::ortho(float l, float r, float b, float t, float n, float f)
{
	return mat4({ 
		2.0f / (r - l),     0.0f,               0.0f,               0.0f,
		0.0f,               2.0f / (t - b),     0.0f,               0.0f,
		0.0f,               0.0f,               -2.0f / (f - n),    0.0f,
		-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1.0f 
	});
}

mat4 mat4::perspective(float aspect, float fov, float n, float f)
{
	return mat4({ 
		1.0f / (aspect * tanf(fov * 0.5f)), 0.0f,                    0.0f,						0.0f,
		0.0f,                               1.0f / tanf(fov * 0.5f), 0.0f,						0.0f,
		0.0f,                               0.0f,                    (f + n) / (n - f),			-1.0f,
		0.0f,                               0.0f,                    (2.0f * f * n) / (n - f) , 0.0f
	});
}

mat4 mat4::operator* (const mat4& rhs) const
{
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
