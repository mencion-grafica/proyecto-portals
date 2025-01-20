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

void Camera::jump()
{
	if (this->position.y <= 0.1f || canJump)
	{
		this->velocity.y = 6.0f;
		canJump = false;
	}
}

void Camera::portal()
{
	return;
}

void Camera::move(float deltaTime)
{
    float speed = 1.5f * deltaTime;

    if (InputManager::keysState[GLFW_KEY_SPACE] && canJump)
    {
        jump();
        canJump = false;
    }

    this->velocity.y += gravity * deltaTime;

    glm::vec3 moveDirection(0.0f);
    if (InputManager::keysState[GLFW_KEY_W]) moveDirection += glm::vec3(this->front.x, 0, this->front.z);
    if (InputManager::keysState[GLFW_KEY_S]) moveDirection -= glm::vec3(this->front.x, 0, this->front.z);
    if (InputManager::keysState[GLFW_KEY_A]) moveDirection -= glm::normalize(glm::cross(this->front, this->up));
    if (InputManager::keysState[GLFW_KEY_D]) moveDirection += glm::normalize(glm::cross(this->front, this->up));

    if (glm::length(moveDirection) > 0.0f) moveDirection = glm::normalize(moveDirection) * speed;

    if (InputManager::keysState[GLFW_KEY_E]) moveDirection += speed * this->up;
    if (InputManager::keysState[GLFW_KEY_Q]) moveDirection -= speed * this->up;

    glm::vec3 predictedPosition = this->position + moveDirection + this->velocity * deltaTime;

    glm::vec3 resolvedPosition = this->position;

    glm::vec3 tempPositionX = resolvedPosition;
    tempPositionX.x = predictedPosition.x;
    if (!Render::r->checkCollisions(glm::vec4(tempPositionX, 1.0f))) resolvedPosition.x = tempPositionX.x;

    glm::vec3 tempPositionY = resolvedPosition;
    tempPositionY.y = predictedPosition.y;
    if (!Render::r->checkCollisions(glm::vec4(tempPositionY, 1.0f)))
    {
        resolvedPosition.y = tempPositionY.y;
        canJump = false;
    }
	else if (this->velocity.y < 0)
    {
        this->velocity.y = 0;
        canJump = true;
    }

    glm::vec3 tempPositionZ = resolvedPosition;
    tempPositionZ.z = predictedPosition.z;
    if (!Render::r->checkCollisions(glm::vec4(tempPositionZ, 1.0f))) resolvedPosition.z = tempPositionZ.z;

    this->position = resolvedPosition;

    if (this->position.y < 0) {
        this->position.y = 0;
        this->velocity.y = 0;
        canJump = true;
    }
}