#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 lookAt, float fovy, float aspectRatio, float zNear, float zFar)
{
	this->orientation = glm::normalize(up);
	this->position = position;
	this->lookAt = lookAt;

	this->fovy = fovy;
	this->aspectRatio = aspectRatio;
	this->zNear = zNear;
	this->zFar = zFar;
}

glm::mat4 Camera::computeViewMatrix()
{
	return glm::lookAt(this->position, this->lookAt, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::computeProjectionMatrix()
{
	return glm::perspective(glm::radians(fovy), this->aspectRatio, this->zNear, this->zFar);
}

void Camera::move(float deltaTime)
{
	float speed = 0.5f * deltaTime;

	if (InputManager::keysState[GLFW_KEY_W]) this->position += speed * this->orientation;
	if (InputManager::keysState[GLFW_KEY_S]) this->position -= speed * this->orientation;
	if (InputManager::keysState[GLFW_KEY_A]) this->position -= glm::normalize(glm::cross(this->orientation, this->lookAt)) * speed;
	if (InputManager::keysState[GLFW_KEY_D]) this->position += glm::normalize(glm::cross(this->orientation, this->lookAt)) * speed;
}