#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

enum CAMERA_MOVEMENT_STATE {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	Camera() = default;
	Camera(float fov, float ratio, float near, float far);
	void updateCam(); // change the cam position
	glm::mat4 getViewMat();
	glm::mat4 getProjectionMat();
	glm::vec3 getCamPosition();
	void setScreenRatio(int w, int h);
	void setCamPosition(CAMERA_MOVEMENT_STATE movDir, float dt);
	void setMouseMovement(float xOffset, float yOffset);
	void setCameraPosition(glm::vec3 pos) { camPosition = pos; }
	void setFrontVector(glm::vec3 front) { camFrontVec = front; }
	void setUpVector(glm::vec3 up) { camFrontVec = up; }
	void updateView(glm::vec3 pos, float dt);
	CAMERA_MOVEMENT_STATE cameraMovementState = CAMERA_MOVEMENT_STATE::BACKWARD;
	void setCameraYaw(float y) { yaw = y; }

private:
	float screenRatio;
	float camFOV;
	float nearPlane;
	float farPlane;
	glm::vec3 camPosition;
	glm::vec3 camUpVec;
	glm::vec3 camFrontVec; // direction
	glm::vec3 camRightVec;
	glm::mat4 camViewMat;
	// euler angles camera movement
	float yaw;
	float pitch;
	// camera options
	float movementSpeed;
	float mouseSensitivity;
	float camDistance;




};