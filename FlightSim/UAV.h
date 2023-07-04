#pragma once
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
const float PI = 3.1415926535;


enum PLANE_STATE
{
	PLANE_FLY,
	PLANE_UP,
	PLANE_DOWN,
	PLANE_ROLL_RIGHT,
	PLANE_ROLL_LEFT,
	PLANE_STOP
};


class UAV : public Mesh
{
public:
	UAV();
	~UAV();
	void update(PLANE_STATE ps, float dt);
	const glm::mat4 getModelMatrix() { return uavTransformationMatrix; };
	const glm::vec3 getModelPosition() { return uavPosition; }
	const glm::vec3 getModelForward() { return uavForwardVec; }
	const glm::vec3 getModelUp() { return uavUpVec; }
	const glm::mat4 getModelRotationMatrix() { return uavRotationMatrix; }
	const glm::mat4 getModelTranslationMatrix() { return uavTranslationMatrix; }
	PLANE_STATE planeState;

	// helper functions 
	// to get sign of a value
	template <typename T>
	constexpr inline T sign(T x)
	{
		return x != 0 ? x / std::abs(x) : 0;
	}

	template <typename T>
	constexpr inline T getSquare(T val)
	{
		return val * val;
	}


private:
	glm::vec3 uavPosition;
	glm::vec3 uavForwardVec;
	glm::vec3 uavUpVec;
	glm::vec3 uavLeftVec;
	glm::vec3 uavVelocity;
	glm::mat4 uavTranslationMatrix;
	glm::mat4 uavRotationMatrix;
	glm::mat4 uavTransformationMatrix;
	float uavSpeed;
	float uavPitchVelocity;
	float uavRollVelocity;

};