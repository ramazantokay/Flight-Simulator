// Ramazan TOKAY
// CG_HW2.cpp 

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <iomanip>
#include <chrono>

#include <GL/glew.h>
//#include <OpenGL/gl3.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/normal.hpp>

//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Renderer.h"
#include "SkyBox.h"
#include "UAV.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
using namespace std;

// Screen size
int width = 1024, height = 768;

Camera* camera;
static int loc; // deneme var

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = (float)width / 2.0f;
float lastY = (float)height / 2.0f;
bool isRightPressed = true;


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

//enum PLANE_STATE
//{
//	PLANE_FLY,
//	PLANE_UP,
//	PLANE_DOWN,
//	PLANE_ROLL_RIGHT,
//	PLANE_ROLL_LEFT
//};

enum GAME_STATE
{
	RUNNING,
	PAUSED,
};

GAME_STATE gameState;
//PLANE_STATE planeState = PLANE_STATE::PLANE_FLY;

UAV* uavModel = new UAV();
unsigned int cubeTexture;
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Close the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	// Wireframe mode
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		cout << "Wireframe" << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	// solid mode
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		cout << "Solid" << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_T == GLFW_PRESS)
	{
		cout << "Pressed T, Front View" << endl;
		camera->cameraMovementState = CAMERA_MOVEMENT_STATE::FORWARD;
		camera->setCameraYaw(90.f);

		/*cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		camera->setCamPosition(FORWARD, deltaTime);*/
	}

	if (key == GLFW_KEY_R == GLFW_PRESS)
	{
		cout << "Pressed R, Back view" << endl;
		camera->cameraMovementState = CAMERA_MOVEMENT_STATE::BACKWARD;
		camera->setCameraYaw(-90.f);
	}

	if (key == GLFW_KEY_Q == GLFW_PRESS)
	{
		cout << "Pressed Q, LEft view" << endl;
		camera->cameraMovementState = CAMERA_MOVEMENT_STATE::LEFT;
		camera->setCameraYaw(-40.f);

	}

	if (key == GLFW_KEY_E == GLFW_PRESS)
	{
		cout << "Pressed E, Right view" << endl;
		camera->cameraMovementState = CAMERA_MOVEMENT_STATE::RIGHT;
		camera->setCameraYaw(-150.f);
	}

	if (key == GLFW_KEY_W == GLFW_PRESS)
	{
		cout << "Pressed UP" << endl;
		
		uavModel->planeState = PLANE_STATE::PLANE_DOWN;
		cout << uavModel->planeState<< endl;
		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		cout << "Released UP" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_FLY;
		cout << uavModel->planeState << endl;

		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}

	if (key == GLFW_KEY_S == GLFW_PRESS)
	{
		cout << "Pressed DOWN" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_UP;
		cout << uavModel->planeState << endl;
		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		cout << "Released DOWN" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_FLY;
		cout << uavModel->planeState << endl;

		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}


	if (key == GLFW_KEY_D == GLFW_PRESS)
	{
		cout << "Pressed RIGHT" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_ROLL_RIGHT;
		cout << uavModel->planeState << endl;
		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		cout << "Released RIGHT" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_FLY;
		cout << uavModel->planeState << endl;

		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}


	if (key == GLFW_KEY_A == GLFW_PRESS)
	{
		cout << "Pressed RIGHT" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_ROLL_LEFT;
		cout << uavModel->planeState << endl;
		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		cout << "Released RIGHT" << endl;
		uavModel->planeState = PLANE_STATE::PLANE_FLY;
		cout << uavModel->planeState << endl;

		//cout << camera->getCamPosition().x << " " << camera->getCamPosition().y << " " << camera->getCamPosition().z << endl;
		//camera->setCamPosition(UP, deltaTime);
	}
	
}

void mouse(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		isRightPressed = true;
		return;
	}

	if (isRightPressed)
	{
		lastX = x;
		lastY = y;
		isRightPressed = false;
	}

	float xOffset = x - lastX;
	float yOffset = lastY-y;
	lastX = x;
	lastY = y;

	camera->setMouseMovement(xOffset, yOffset);
	
}

void reshape(GLFWwindow* window, int w, int h)
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;


	glViewport(0, 0, w, h);
	
	// Camera ratio setting 
	camera->setScreenRatio(w, h);


}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

}

void mainLoop(GLFWwindow* window, UAV* uavModel, Renderer* uavRenderer,  Renderer* airportRender, Renderer* canopyRenderer, Renderer* groundRenderer, SkyBox* skyRenderer,Camera* cam)
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		nbFrames++;
		// print FPS count 
		if (currentTime - lastTime >= 1.0) {
			string fr = " -- FPS: " + to_string(nbFrames);
			char rendererInfo[512] = { 0 };
			strcpy(rendererInfo, (const char*)glGetString(GL_RENDERER));
			(rendererInfo, (const char*)glGetString(GL_RENDERER));
			strcat(rendererInfo, " - ");
			strcat(rendererInfo, (const char*)glGetString(GL_VERSION));
			strcat(rendererInfo, fr.c_str());
			glfwSetWindowTitle(window, rendererInfo);
			nbFrames = 0;
			lastTime += 1.0;
		}

		// calculate dt
		float currFrame = static_cast<float>(glfwGetTime());
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;
		const std::chrono::steady_clock::duration frame_period(std::chrono::milliseconds(1000 / 60));
		const float dt = std::chrono::duration<float>(frame_period).count();

		cout << "dt " << dt << endl;
		//glClearColor(0, 0, 0, 1);
		glClearStencil(0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		glm::mat4 airportModel = glm::translate(glm::vec3(40.0f, 0.0f, -150.0f)) * glm::rotate<float>(glm::mat4(1.0), glm::radians(90.0), glm::vec3(0,1, 0)) * glm::scale(glm::vec3(1.0f));
		
		glm::mat4 canopyModel = (glm::translate(glm::vec3(0.0f, 0.1f, 0.0f)) * uavModel->getModelTranslationMatrix())*uavModel->getModelRotationMatrix();
		//glm::mat4 canopyModel = uavModel->getModelMatrix();
		
		glm::mat4 groundModel= glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate<float>(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(500.0f));
		
		// update uav status, then render the status with uavRenderer
		uavModel->update(uavModel->planeState, dt);


		// camera update
		cam->updateView (uavModel->getModelPosition(), dt);

		glm::mat4 VP = cam->getProjectionMat() * cam->getViewMat();

		//glm::mat4 view = cam->getViewMat();

		// skybox renderer set view then render
		skyRenderer->bind();
		skyRenderer->renderCubeFaces();

		skyRenderer->setUniformMatrix("ProjectionMatrix", cam->getProjectionMat());
		skyRenderer->setUniformMatrix("ViewMatrix", glm::mat4(glm::mat3(cam->getViewMat()))); // this very critic, remove translation from the view matrix
		skyRenderer->render();

		// uav drone renderer
		uavRenderer->bind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyRenderer->getCubeMap());
		uavRenderer->setUniformMatrix("u_VP", VP);
		uavRenderer->setUniformMatrix("u_NormalTransform", glm::inverse(glm::transpose(uavModel->getModelMatrix())));
		uavRenderer->setUniformMatrix("u_ModelMatrix", uavModel->getModelMatrix());
		uavRenderer->setUniformVec3("u_CameraPosition", cam->getCamPosition());
		uavRenderer->setUniformVec3("u_Color", glm::vec3{ 0.5f, 0.1f, 0.1f });
		uavRenderer->render();
		
		// airport renderer
		airportRender->bind();
		airportRender->setUniformMatrix("u_VP", VP);
		airportRender->setUniformMatrix("u_NormalTransform", glm::inverse(glm::transpose(airportModel)));
		airportRender->setUniformMatrix("u_ModelMatrix", airportModel);
		airportRender->setUniformVec3("u_CameraPosition", cam->getCamPosition());
		airportRender->setUniformVec3("u_Color", glm::vec3{ 0.1f, 0.1f, 0.1f });
		airportRender->render();

		// groundPlane renderer
		groundRenderer->bind();
		groundRenderer->setUniformMatrix("u_VP", VP);
		groundRenderer->setUniformMatrix("u_NormalTransform", glm::inverse(glm::transpose(groundModel)));
		groundRenderer->setUniformMatrix("u_ModelMatrix", groundModel);
		groundRenderer->setUniformVec3("u_CameraPosition", cam->getCamPosition());
		groundRenderer->setUniformVec3("u_Color", glm::vec3{ 0.1f, 0.5f, 0.1f });
		groundRenderer->render();


		//// canopy renderer
		canopyRenderer->bind();
		canopyRenderer->setUniformMatrix("u_VP", VP);
		canopyRenderer->setUniformMatrix("u_NormalTransform", glm::inverse(glm::transpose(canopyModel)));
		canopyRenderer->setUniformMatrix("u_ModelMatrix", canopyModel);
		canopyRenderer->setUniformVec3("u_CameraPosition", cam->getCamPosition());
		canopyRenderer->setUniformVec3("u_Color", glm::vec3{ 0.4f, 0.5f, 0.3f });
		canopyRenderer->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "CG HW2", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	glfwSetCursorPosCallback(window, mouse);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	init();

	// Camera init
	glfwSetWindowSizeCallback(window, reshape);
	glfwSetKeyCallback(window, keyboard);

	//------------------------------------------- INIT --------------------------------------------------------//
	// 
	// UAV init
	// UAV load obj file
	if (!uavModel->loadObj("models/f16.obj"))
		cout << "Wowo wowo" << endl;
	else
		cout << "Niceee" << endl;
	//uavModel->planeState = PLANE_STATE::PLANE_STOP;
	// UAV 
	Shader* uavShader = new Shader("shaders/uav.glsl");

	Renderer* uavRenderer = new Renderer(uavModel, uavShader);
	uavRenderer->init();
	uavRenderer->bind();
	//
	
	// AIRPORT
	Mesh* airport = new Mesh();
	if (!airport->loadObj("models/airport.obj"))
		cout << "Wowo wowo" << endl;
	else
		cout << "Niceee" << endl;
	Shader* airportShader = new Shader("shaders/airport.glsl");

	Renderer* airportRenderer = new Renderer(airport, airportShader);
	airportRenderer->init();
	airportRenderer->bind();
	//

	// CANOPY
	Mesh* canopy = new Mesh();
	if (!canopy->loadObj("models/canopy.obj"))
		cout << "Wowo wowo" << endl;
	else
		cout << "Niceee" << endl;
	Shader* canopyShader = new Shader("shaders/canopy.glsl");

	Renderer* canopyRenderer = new Renderer(canopy, canopyShader);
	canopyRenderer->init();
	canopyRenderer->bind();
	//

	// ground plane
	Mesh* groundPlane = new Mesh();
	if (!groundPlane->loadObj("models/ground.obj"))
		cout << "Wowo wowo" << endl;
	else
		cout << "Niceee" << endl;
	Shader* groundPlaneShader = new Shader("shaders/ground.glsl");

	Renderer* groundPlaneRenderer = new Renderer(groundPlane, groundPlaneShader);
	groundPlaneRenderer->init();
	groundPlaneRenderer->bind();
	groundPlaneRenderer->loadTexture("textures/ground_texture_grass.jpg");
	//

	camera = new Camera(45.0f, (float)width / (float)height, 1.0f, 1000.0f);

	// Skybox Shader and Renderer
	Shader* skyBoxShader = new Shader("shaders/skybox.glsl");
	SkyBox* skyBoxRenderer = new SkyBox(skyBoxShader, uavModel, airportRenderer,groundPlaneRenderer, uavRenderer, camera);
	skyBoxRenderer->initVBO();
	skyBoxRenderer->initTextures();
	skyBoxRenderer->initFBO();
	skyBoxRenderer->bind();
	//
	reshape(window, width, height); // need to call this once ourselves


#pragma region airport_mesh
	
#pragma endregion

	mainLoop(window, uavModel, uavRenderer, airportRenderer, canopyRenderer, groundPlaneRenderer, skyBoxRenderer, camera); // this does not return unless the window is closed

	glfwDestroyWindow(window);
	glfwTerminate();

	// deletes
	delete camera;
	
	delete uavModel;
	delete airport;
	delete groundPlane;
	delete canopy;
	

	delete uavShader;
	delete uavRenderer;
	delete canopyRenderer;
	delete groundPlaneRenderer;

	delete skyBoxShader;
	delete skyBoxRenderer;


}
