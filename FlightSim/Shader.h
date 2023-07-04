#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>

class Shader
{
protected:
	enum class ShaderType
	{
		NONE = 0,
		VERTEX = 1,
		FRAGMENT = 2
	};
	struct ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
		ShaderProgramSource()
		{
			vertexSource = fragmentSource = "";
		}
		ShaderProgramSource(const std::string& vertexSource, const std::string& fragmentSource)
		{
			this->vertexSource = vertexSource;
			this->fragmentSource = fragmentSource;
		}
	};

	virtual uint32_t createShader(const std::string filePath);
	static uint32_t compileShader(const std::string& source, uint32_t type);
	static ShaderProgramSource parseShader(const std::string& filePath);

public:
	Shader() = default;
	Shader(const std::string& filePath);
	~Shader();
	void SetUpShader(const std::string& filePath);
public:
	uint32_t program;
};