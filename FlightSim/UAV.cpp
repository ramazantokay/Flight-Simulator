#include <cmath>
#include "UAV.h"

UAV::UAV()
{
	planeState = PLANE_STATE::PLANE_FLY;
	uavUpVec = { 0.0f,1.0f,0.0f };
	uavForwardVec = { 0.0f,0.0f,-1.0f };
	uavLeftVec = { -1.0f,0.0f,0.f };
	uavPosition = { 0.0f, 40.0f, 200.f };
	uavSpeed = 1.0f;
	uavVelocity = { glm::vec3(1.0f) * uavSpeed };
	uavPitchVelocity = 0.0f;
	uavRollVelocity = 0.0f;
	uavRotationMatrix = glm::mat4(1.0f);
	uavTranslationMatrix = glm::translate(glm::mat4(1.f), uavPosition);
	uavTransformationMatrix = uavTranslationMatrix * uavRotationMatrix;
}

UAV::~UAV()
{
}

void UAV::update(PLANE_STATE ps, float dt)
{
	if (planeState != PLANE_STOP)
	{
		uavSpeed += 0.5 * dt;
		uavSpeed = std::min(std::max(0.0f, uavSpeed), 15.0f);

		float targetSpeed, yawPitchrate;
		bool isPositive;
	
		using ClampFunction = const float& (*)(const float&, const float&);

		// ROLL
		if (planeState == PLANE_ROLL_LEFT)
		{
			targetSpeed = (PI / 3.f) * 1;
			isPositive = true;
			yawPitchrate = 0.004f;
		}
		else if (planeState == PLANE_ROLL_RIGHT)
		{
			targetSpeed = (PI / 3.f) * -1;
			isPositive = false;
			yawPitchrate = 0.004f;
		}
		else
		{
			targetSpeed = sign(uavUpVec.y) * -sign(uavLeftVec.y) * std::sqrt(std::abs(uavLeftVec.y)) * PI / 6.f;
			isPositive = sign(uavUpVec.y) * -sign(uavLeftVec.y) > 0;
			yawPitchrate = 0.015f;
		}

		auto	clamping_func = isPositive ? ClampFunction(std::min) : ClampFunction(std::max);
		uavRollVelocity = clamping_func(uavRollVelocity + targetSpeed * yawPitchrate, targetSpeed);
		float rollAngle = uavRollVelocity * dt;

		if (std::abs(rollAngle) > 1e-4)
			uavRotationMatrix = glm::rotate(uavRotationMatrix, rollAngle, { 0.f, 0.f, 1.f });

		//cout << "uavRotationMatrixX: " << uavRotationMatrix[0].x << " " << uavRotationMatrix[0].y << " " << uavRotationMatrix[0].z << endl;
		//cout << "uavRotationMatrixY: " << uavRotationMatrix[1].x << " " << uavRotationMatrix[1].y << " " << uavRotationMatrix[1].z << endl;
		//cout << "uavRotationMatrixZ: " << uavRotationMatrix[2].x << " " << uavRotationMatrix[2].y << " " << uavRotationMatrix[2].z << endl;


		// Pitch
		if (planeState == PLANE_DOWN)
		{
			targetSpeed = (PI / 6.f * (1.f - ((uavLeftVec.y)))) * -1;
			isPositive = false;
			yawPitchrate = 0.004f;

		}
		else if (planeState == PLANE_UP)
		{
			targetSpeed = (PI / 6.f * (1.f - ((uavLeftVec.y)))) * 1;
			isPositive = true;
			yawPitchrate = 0.004f;
		}
		else
		{
			targetSpeed = sign(uavUpVec.y) * sign(uavForwardVec.y) * std::sqrt(std::abs(uavForwardVec.y)) * PI / 6.f;
			isPositive = sign(uavUpVec.y) * sign(uavForwardVec.y) > 0;
			yawPitchrate = 0.003f;
		}

		clamping_func = isPositive ? ClampFunction(std::min) : ClampFunction(std::max);
		uavPitchVelocity = clamping_func(uavPitchVelocity + targetSpeed * yawPitchrate, targetSpeed);
		float pitchAngle = uavPitchVelocity * dt;

		if (std::abs(pitchAngle) > 1e-4)
			uavRotationMatrix = glm::rotate(uavRotationMatrix, pitchAngle, { 1.f, 0.f, 0.f });

		uavLeftVec = glm::normalize(uavRotationMatrix[0]);
		uavUpVec = glm::normalize(uavRotationMatrix[1]);
		uavForwardVec = glm::normalize(uavRotationMatrix[2]);

		// https://aircraftflightmechanics.com/AircraftPerformance/Introduction.html for more aircraft equation ,
		// for the simplicity calculated drag, lift, gravity and thrust rates

		auto thrustRate = uavForwardVec * 15.0f * uavSpeed / 1.0f;
		auto dragRate = -glm::normalize(uavVelocity) * (15.0f / getSquare(1.0f)) * glm::dot(uavVelocity, uavVelocity);
		glm::vec3 liftRate = { 0.f, (uavUpVec * (2.f / 1.0f) * getSquare(glm::dot(uavForwardVec, uavVelocity))).y, 0.f };
		glm::vec3 gravityValue = glm::vec3(0, -1, 0) * 2.f;

		glm::vec3 accelerationRate = thrustRate + dragRate + liftRate + gravityValue;
		//glm::vec3 acceleration = thrust_wtf + drag_wtf ;

		uavVelocity += accelerationRate * dt;
		uavPosition -= uavVelocity * dt;
	}

	if (uavPosition.y <= 2.f)
	{
		uavPosition.y = 2.f;
		uavSpeed -= 1 * dt;
		if (uavSpeed <= 1.f)
		{
			planeState = PLANE_STOP;
			uavSpeed = 0.f;
		}
	}

	uavTranslationMatrix = glm::translate(glm::mat4(1.f), uavPosition);
	uavTransformationMatrix = uavTranslationMatrix * uavRotationMatrix;
	planeState == PLANE_FLY;
	cout << "Speed: " << uavSpeed << endl;
	cout << "uavPosition: " << uavPosition.x << " " << uavPosition.y << " " << uavPosition.z << endl;
}
