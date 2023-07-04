#include "stb_image.h"

#include "Renderer.h"


Renderer::~Renderer()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &gVertexAttribBuffer);
	glDeleteBuffers(1, &gIndexBuffer);
}

void Renderer::setUniformMatrix(const std::string& uniformName, const glm::mat4& matrix)
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
	UniformLocation& uniformLocation = uniformLocations[uniformName];
	if (!uniformLocation.isAlreadyTaken)
	{
		uniformLocation.location = glGetUniformLocation(program->program, uniformName.c_str());

	}
	glUniformMatrix4fv(uniformLocation.location, 1, GL_FALSE, &matrix[0][0]);
}

void Renderer::setUniformVec3(const std::string& uniformName, const glm::vec3& vec)
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
	UniformLocation& uniformLocation = uniformLocations[uniformName];
	if (!uniformLocation.isAlreadyTaken)
	{
		uniformLocation.location = glGetUniformLocation(program->program, uniformName.c_str());

	}

	glUniform3fv(uniformLocation.location, 1, &vec[0]); // TODO check opengl extensions
}

void Renderer::setUniform1i(const std::string& uniformName, int32_t value)
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
	UniformLocation& uniformLocation = uniformLocations[uniformName];
	if (!uniformLocation.isAlreadyTaken)
	{
		uniformLocation.location = glGetUniformLocation(program->program, uniformName.c_str());

	}
	glUniform1i(uniformLocation.location, value);
}

void Renderer::loadTexture(const std::string& texturePath)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 4);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << texturePath << std::endl;
		stbi_image_free(data);
	}
	texture = textureID;
}

void Renderer::bind()
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
}

Renderer::Renderer(Mesh* mesh, Shader* shader)
{
	this->mesh = mesh;
	this->program = shader;
}

void Renderer::init()
{
	glGenVertexArrays(1, &VAO); // generate vertex array object
	assert(VAO > 0);
	glBindVertexArray(VAO);
	cout << "vao = " << VAO << endl;

	int gVertexDataSizeInBytes = mesh->gVertices.size() * 3 * sizeof(GLfloat);
	int gNormalDataSizeInBytes = mesh->gNormals.size() * 3 * sizeof(GLfloat);
	int indexDataSizeInBytes = mesh->gFaces.size() * 3 * sizeof(GLuint);
	int textureDataSizeInBytes = mesh->gTextures.size() * 2 * sizeof(GLuint);

	GLfloat* vertexData = new GLfloat[mesh->gVertices.size() * 3];
	GLuint* indexData = new GLuint[mesh->gFaces.size() * 3];
	GLfloat* normalData = new GLfloat[mesh->gNormals.size() * 3];
	GLfloat* textureData = new GLfloat[mesh->gTextures.size() * 2];

	float minX = 1e6, maxX = -1e6;
	float minY = 1e6, maxY = -1e6;
	float minZ = 1e6, maxZ = -1e6;

	for (int i = 0; i < mesh->gVertices.size(); i++)
	{
		vertexData[(3 * i)] = mesh->gVertices[i].x;
		vertexData[(3 * i) + 1] = mesh->gVertices[i].y;
		vertexData[(3 * i) + 2] = mesh->gVertices[i].z;
		minX = std::min(minX, mesh->gVertices[i].x);
		maxX = std::max(maxX, mesh->gVertices[i].x);
		minY = std::min(minY, mesh->gVertices[i].y);
		maxY = std::max(maxY, mesh->gVertices[i].y);
		minZ = std::min(minZ, mesh->gVertices[i].z);
		maxZ = std::max(maxZ, mesh->gVertices[i].z);
		//cout << "x: " << vertexData[(3 * i)] << " y: " << vertexData[(3 * i) + 1] << " z: " << vertexData[(3 * i) + 2] << endl;
	}

	std::cout << "minX = " << minX << std::endl;
	std::cout << "maxX = " << maxX << std::endl;
	std::cout << "minY = " << minY << std::endl;
	std::cout << "maxY = " << maxY << std::endl;
	std::cout << "minZ = " << minZ << std::endl;
	std::cout << "maxZ = " << maxZ << std::endl;

	for (int i = 0; i < mesh->gFaces.size(); i++)
	{
		indexData[3 * i] = mesh->gFaces[i].vIndex[0];
		indexData[3 * i + 1] = mesh->gFaces[i].vIndex[1];
		indexData[3 * i + 2] = mesh->gFaces[i].vIndex[2];
	}

	for (int i = 0; i < mesh->gNormals.size(); i++)
	{
		normalData[3 * i] = mesh->gNormals[i].x;
		normalData[3 * i + 1] = mesh->gNormals[i].y;
		normalData[3 * i + 2] = mesh->gNormals[i].z;
	}

	for (int i = 0; i < mesh->gTextures.size(); i++)
	{
		textureData[2 * i] = mesh->gTextures[i].u;
		textureData[2 * i + 1] = mesh->gTextures[i].v;
	}


	glGenBuffers(1, &gVertexAttribBuffer);
	glGenBuffers(1, &gIndexBuffer);

	assert(gVertexAttribBuffer > 0 && gIndexBuffer > 0);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NONE);

	glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer); //vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer); //ebo

	glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes+ textureDataSizeInBytes, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
	glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
	glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes+ gNormalDataSizeInBytes, textureDataSizeInBytes, textureData); 


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //vertices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));//normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes+ gNormalDataSizeInBytes));// textures

	delete[] vertexData;
	delete[] indexData;
	delete[] normalData;
	delete[] textureData;

}

void Renderer::render()
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

	}
	glDrawElements(GL_TRIANGLES, mesh->gFaces.size() * 3, GL_UNSIGNED_INT, 0);
}