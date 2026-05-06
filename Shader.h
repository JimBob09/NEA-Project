#pragma once
#include <iostream>
#include "vec.h"
#include "mat4.h"

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader() { glDeleteProgram(m_id); }

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void use() const { glUseProgram(m_id); }

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, const vec4& value) const;
	void setMat4(const std::string& name, const mat4& mat) const;

	unsigned int id() const { return m_id; }

private:
	unsigned int m_id;

	std::string readFile(const std::string& path);
	unsigned int compileShader(unsigned int type, const std::string& source);
};