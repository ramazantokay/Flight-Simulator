#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Shader.h"
#include <unordered_map>
#include "Renderer.h"
#include "Camera.h"
#include "UAV.h"


class SkyBox
{
public:
	SkyBox(Shader* shader, UAV* uavModel, Renderer* airport, Renderer* ground, Renderer* uav, Camera* cam);
	~SkyBox();
	void initVBO();
	void initTextures();
	void initFBO(); // where the dynamic cube map init
	void bind();
	void render();
	void renderCubeFaces();
	void setUniformMatrix(const std::string& uniformName, const glm::mat4& matrix);
	void setUniformVec3(const std::string& uniformName, const glm::vec3& vec);
	void SetUniform1i(const std::string& uniformName, int32_t value);
	GLuint getCubeMap() { return cubeMap; }
private:
	GLuint VBO, VAO, cubeTexture, cubeMap, frameBuffer, depthBuffer;
	Shader* program;
	Renderer* ground, * uav, *airport;
	Camera* camera;
	UAV* uavModel;

//std::vector<std::string> textureLocations = {
//"textures/skybox_texture_test/right.jpg",
//"textures/skybox_texture_test/left.jpg",
//"textures/skybox_texture_test/top.jpg" ,
//"textures/skybox_texture_test/bottom.jpg" ,
//"textures/skybox_texture_test/front.jpg" ,
//"textures/skybox_texture_test/back.jpg" };
//
	std::vector<std::string> textureLocations = {
"textures/skybox_texture_sea/right.jpg",
"textures/skybox_texture_sea/left.jpg",
"textures/skybox_texture_sea/top.jpg" ,
"textures/skybox_texture_sea/bottom.jpg" ,
"textures/skybox_texture_sea/front.jpg" ,
"textures/skybox_texture_sea/back.jpg" };
//	std::vector<std::string> textureLocations = {
//"textures/skybox_texture_ruins/right.png",
//"textures/skybox_texture_ruins/left.png",
//"textures/skybox_texture_ruins/top.png" ,
//"textures/skybox_texture_ruins/bottom.png" ,
//"textures/skybox_texture_ruins/front.png" ,
//"textures/skybox_texture_ruins/back.png" };
//	std::vector<std::string> textureLocations = {
//"textures/skybox_texture_1/right.jpg",
//"textures/skybox_texture_1/left.jpg",
//"textures/skybox_texture_1/top.jpg" ,
//"textures/skybox_texture_1/bottom.jpg" ,
//"textures/skybox_texture_1/front.jpg" ,
//"textures/skybox_texture_1/back.jpg" };
private:
	struct UniformLocation
	{
		bool isAlreadyTaken;
		uint32_t location;
	};
	std::unordered_map<std::string, UniformLocation> uniformLocations;
};