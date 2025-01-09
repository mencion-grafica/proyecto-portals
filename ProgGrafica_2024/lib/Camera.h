#pragma once

#include "Common.h"
#include "InputManager.h"

class Camera
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);

	bool canJump = false;
	float gravity = -9.8f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;
	float zNear = 0.1f;
	float zFar = 10000.0f;
	float aspectRatio = 800.0f / 600.0f;
	bool firstMouse = true;

	Camera();

	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov, float aspectRatio);
	
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);

	void jump();

	void portal();

	glm::mat4 computeViewMatrix();

	glm::mat4 computeProjectionMatrix();

	virtual void move(float deltaTime);
};