#pragma once
#include <span>
#include "vec.h"

struct Vertex
{
	vec3 position;
	vec2 texCoord;
};

class Mesh
{
public:
	Mesh(std::span<const Vertex> vertices, std::span<const unsigned int> indices);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void draw() const;

private:
	unsigned int m_VAO, m_VBO, m_EBO;
	size_t m_indexCount;
};