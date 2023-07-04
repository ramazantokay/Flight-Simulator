#include "Shader.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdlib.h>

Shader::Shader(const std::string& filePath)
{
	program = createShader(filePath);
}

void Shader::SetUpShader(const std::string& filePath)
{
	program = createShader(filePath);
}

uint32_t Shader::createShader(const std::string filePath)
{
	uint32_t program = glCreateProgram();
	ShaderProgramSource source = parseShader(filePath);
	uint32_t vs = compileShader(source.vertexSource, GL_VERTEX_SHADER);
	uint32_t fs = compileShader(source.fragmentSource, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


uint32_t Shader::compileShader(const std::string& source, uint32_t type)
{
	uint32_t id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length + 1];
		message[length] = 0;
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : (type == GL_FRAGMENT_SHADER ? "fragment" : "geometry"))
			<< " shader!\n" << message << "\n";
		delete[] message;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

Shader::ShaderProgramSource Shader::parseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	std::string line;
	ShaderType type = ShaderType::NONE;
	std::stringstream ss[3];
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
			
		}
		else
		{
			ss[(int)type - 1] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };

}


Shader::~Shader()
{
	glDeleteProgram(program);
}