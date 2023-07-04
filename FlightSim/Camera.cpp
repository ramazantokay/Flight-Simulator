#include "Camera.h"

Camera::Camera(float fov, float ratio, float near, float far)
{
	camFOV = fov;
	screenRatio = ratio;
	nearPlane = near;
	farPlane = far;
	mouseSensitivity = 0.1f;
	movementSpeed = 150.0f;
	yaw = -90.0f;
	pitch = 0.0f;
	camDistance = 0.5f;
	//
	//camPosition = { -10.0f, 80.0f, 150.0f }; // pos
	camPosition = { 0.0f,0.0f,0.f }; // pos
	camFrontVec = { 0.0f, 0.0f, -1.0f }; // front
	camUpVec = { 0.0f, 1.0f, 0.0f }; // up
	camRightVec = glm::normalize(glm::cross(camFrontVec, camUpVec));
	camViewMat = glm::lookAt(camPosition, camPosition + camFrontVec, camUpVec); // view
}

void Camera::updateCam()
{
	// mouse callback 
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tempFront.y = sin(glm::radians(pitch));
	tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFrontVec = glm::normalize(tempFront);

	camRightVec = glm::normalize(glm::cross(camFrontVec, glm::vec3(0.0f, 1.0f, 0.0f)));
	camUpVec = glm::normalize(glm::cross(camRightVec, camFrontVec));
	//std::cout << "x: "<< pitch << " y: " << yaw << std::endl;
}

glm::mat4 Camera::getViewMat()
{
	return  glm::lookAt(camPosition, camPosition + camFrontVec, camUpVec);
}

glm::mat4 Camera::getProjectionMat()
{
	return glm::perspective(glm::radians(camFOV), screenRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getCamPosition()
{
	return camPosition;
}

void Camera::setScreenRatio(int w, int h)
{
	screenRatio = (float)w / (float)h;
}

void Camera::setCamPosition(CAMERA_MOVEMENT_STATE movDir, float dt)
{
	//const float cameraSpeed = movementSpeed * dt;
	//if (movDir == FORWARD)
	//	camPosition += camFrontVec * cameraSpeed;
	//if (movDir == BACKWARD)
	//	camPosition -= camFrontVec * cameraSpeed;
	//if (movDir == RIGHT)
	//	camPosition += camRightVec * cameraSpeed;
	//if (movDir == LEFT)
	//	camPosition -= camRightVec * cameraSpeed;
	//if (movDir == DOWN)
	//	camPosition -= camUpVec * cameraSpeed;
	//if (movDir == UP)
	//	camPosition += camUpVec * cameraSpeed;
}

void Camera::setMouseMovement(float xOffset, float yOffset)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCam();
}

void Camera::updateView(glm::vec3 pos, float dt)
{
	//camPosition = pos + glm::vec3(-10.0f, 60.0f, 250.0f);

	if (cameraMovementState== BACKWARD)
	{
		camPosition = pos + (glm::vec3(0.0f, 10.0f,40.0f));
		//yaw = -90.0f;
		updateCam();
	}
	if (cameraMovementState == FORWARD)
	{
		camPosition = pos + glm::vec3(0.0f, 10.0f, -80.0f);
		//yaw = 90.0f;
		updateCam();
	}
	if (cameraMovementState == LEFT)
	{
		camPosition = pos + glm::vec3(-100.0f, 0.0f, 40.0f);
		//yaw = -40.0f;
		updateCam();
	}
	if (cameraMovementState == RIGHT)
	{
		camPosition = pos + glm::vec3(100.0f, 0.0f, 40.0f);
		//yaw = -150.0f;
		updateCam();
	}
	



}
