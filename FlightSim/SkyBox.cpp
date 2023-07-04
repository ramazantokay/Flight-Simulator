#include "SkyBox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SkyBox::SkyBox(Shader* shader, UAV* uavModel, Renderer* airport, Renderer* ground, Renderer* uav, Camera* cam)
{
	this->program = shader;
	this->uavModel = uavModel;
	this->ground = ground;
	this->uav = uav;
	this->camera = cam;
	this->airport = airport;
}

SkyBox::~SkyBox()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &cubeTexture);
	glDeleteBuffers(1, &cubeMap);
	glDeleteBuffers(1, &frameBuffer);
	glDeleteBuffers(1, &depthBuffer);
}

void SkyBox::initVBO()
{
	float skyBoxVerts[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), skyBoxVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnable(GL_DEPTH_TEST);

}

void SkyBox::initTextures()
{
	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < textureLocations.size(); i++)
	{
		unsigned char* data = stbi_load(textureLocations[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << textureLocations[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void SkyBox::initFBO()
{
	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubeMap, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
void SkyBox::bind()
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
}

void SkyBox::render()
{
	glUseProgram(program->program);
	glBindVertexArray(VAO);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void SkyBox::renderCubeFaces()
{
	// Render all sides of the cube

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	glViewport(0, 0, 1024, 1024);


	for (int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 1000.0f);
		glm::mat4 viewMatrix;
		switch (i)
		{
		case 0:
			viewMatrix = glm::lookAt(uavModel->getModelPosition(), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 1:
			viewMatrix = glm::lookAt(uavModel->getModelPosition(), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 2:
			viewMatrix = glm::lookAt(uavModel->getModelPosition(), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case 3:
			viewMatrix = glm::lookAt(uavModel->getModelPosition(), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			break;
		case 4:
			viewMatrix = glm::lookAt(uavModel->getModelPosition(), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		case 5:
			viewMatrix = glm::lookAt(uavModel->getModelPosition(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			break;
		}

		// Render SKYBOX
		glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		glUseProgram(program->program);
		setUniformMatrix("ProjectionMatrix", -projectionMatrix);
		setUniformMatrix("ViewMatrix", glm::mat4(glm::mat3(viewMatrix))); // this very critic, remove translation from the view matrix
		glBindVertexArray(VAO);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);


		glm::mat4 VP = projectionMatrix * viewMatrix;
		glm::mat4 airportModel = glm::translate(glm::vec3(40.0f, 0.0f, -150.0f)) * glm::rotate<float>(glm::mat4(1.0), glm::radians(90.0), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f));
		glm::mat4 groundModel = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate<float>(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(500.0f));

		// airport renderer
		airport->bind();
		airport->setUniformMatrix("u_VP", VP);
		airport->setUniformMatrix("u_NormalTransform", glm::inverse(glm::transpose(airportModel)));
		airport->setUniformMatrix("u_ModelMatrix", airportModel);
		airport->setUniformVec3("u_CameraPosition", camera->getCamPosition());
		airport->setUniformVec3("u_Color", glm::vec3{ 0.1f, 0.1f, 0.1f });
		airport->render();

		// Render ground

		ground->bind();
		ground->setUniformMatrix("u_VP", VP);
		ground->setUniformMatrix("u_NormalTransform", glm::inverse(glm::transpose(groundModel)));
		ground->setUniformMatrix("u_ModelMatrix", groundModel);
		ground->setUniformVec3("u_CameraPosition", camera->getCamPosition());
		ground->setUniformVec3("u_Color", glm::vec3{ 0.1f, 0.5f, 0.1f });
		ground->render();

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glViewport(0, 0, 1024, 768);
}


void SkyBox::setUniformMatrix(const std::string& uniformName, const glm::mat4& matrix)
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

void SkyBox::setUniformVec3(const std::string& uniformName, const glm::vec3& vec)
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

void SkyBox::SetUniform1i(const std::string& uniformName, int32_t value)
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