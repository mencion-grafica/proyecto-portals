#include "Camera.h"

#include "Render.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov, float aspectRatio)
{
	this->position = position;
	this->front = front;
	this->up = up;

	this->fov = fov;
	this->aspectRatio = aspectRatio;
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	this->position = position;
	this->front = front;
	this->up = up;
}

glm::mat4 Camera::computeViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

glm::mat4 Camera::computeProjectionMatrix()
{
	return glm::perspective(glm::radians(this->fov), this->aspectRatio, this->zNear, this->zFar);
}

void Camera::move(float deltaTime)
{
	float speed = 50.0f * deltaTime;

	if (InputManager::keysState[GLFW_KEY_W]) this->position += speed * this->front;
	if (InputManager::keysState[GLFW_KEY_S]) this->position -= speed * this->front;
	if (InputManager::keysState[GLFW_KEY_A]) this->position -= glm::normalize(glm::cross(this->front, this->up)) * speed;
	if (InputManager::keysState[GLFW_KEY_D]) this->position += glm::normalize(glm::cross(this->front, this->up)) * speed;

	if (Render::r->checkCollisions(glm::vec4(this->position, 1.0f))) this->position += speed * this->up;
	if (InputManager::keysState[GLFW_KEY_E]) this->position += speed * this->up;
	if (InputManager::keysState[GLFW_KEY_Q]) this->position -= speed * this->up;
}