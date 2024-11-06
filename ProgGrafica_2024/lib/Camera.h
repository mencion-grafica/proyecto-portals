#pragma once

#include "Common.h"
#include "InputManager.h"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 lookAt;

	float fovy = 0.0f;
	float aspectRatio = 0.0f;
	float zNear = 0;
	float zFar = 0;

	Camera(glm::vec3 position, glm::vec3 up, glm::vec3 lookAt, float fovy, float aspectRatio, float zNear, float zFar);

	glm::mat4 computeViewMatrix();

	glm::mat4 computeProjectionMatrix();

	virtual void move(float deltaTime);
};