#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
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
	void setVec3(const std::string& name, const vec3& value) const;
	void setVec4(const std::string& name, const vec4& value) const;
	void setMat4(const std::string& name, const mat4& mat) const;

	constexpr unsigned int id() const { return m_id; }

private:
	unsigned int m_id;
	mutable std::unordered_map<std::string, int> m_uniformLocationCache;

	int getUniformLocation(const std::string& name) const;

	static std::string readFile(const std::string& path);
	static unsigned int compileShader(unsigned int type, const std::string& source);
};