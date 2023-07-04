#pragma once
#include <iostream>
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <unordered_map>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
using std::min;
using std::max;

class Renderer
{

public:
	Renderer() = default;
	Renderer(Mesh* mesh, Shader* shader);
	~Renderer();
	void init();
	void bind();
	void render();
	void setUniformMatrix(const std::string& uniformName, const glm::mat4& matrix);
	void setUniformVec3(const std::string& uniformName, const glm::vec3& vec);
	void setUniform1i(const std::string& uniformName, int32_t value);
	void loadTexture(const std::string& texturePath);
public:
	Mesh* mesh;
	Shader* program;
	GLuint VAO, gVertexAttribBuffer, gIndexBuffer, gTextureBuffer, texture;

private:
	struct UniformLocation
	{
		bool isAlreadyTaken;
		uint32_t location;
	};
	std::unordered_map<std::string, UniformLocation> uniformLocations;
};
