#include "Camera.h"

#include "Render.h"

Camera::Camera()
{
	this->fbt = new FrameBufferTexture(1024, 768);
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov, float aspectRatio, Texture *tex)
{
	this->position = position;
	this->front = front;
	this->up = up;

	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->fbt = new FrameBufferTexture(1024, 768);
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
	if(this->followCamera == nullptr){
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
	else
	{
		// Calculate the relative position of the follow camera to the portal position
		glm::vec3 relativePosition = this->followCamera->position - this->portalPosition;

		// Rotate the relative position by the yaw offset
		float yawRadians = glm::radians(-this->yawOffset);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), yawRadians, this->up);
		glm::vec3 rotatedPosition = glm::vec3(rotationMatrix * glm::vec4(relativePosition, 1.0f));

		// Set the new position of the portal camera
		this->position = glm::vec3(this->pivotPosition) + rotatedPosition;
		
		
		// Adjust the yaw and pitch of the portal camera
		this->yaw = this->followCamera->yaw + this->yawOffset;
		this->pitch = this->followCamera->pitch;

		// Recalculate the front vector
		this->front = glm::vec3(cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
								sin(glm::radians(this->pitch)),
								sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));
		this->front = glm::normalize(this->front);
		
		/*float diff = glm::distance(this->portalPosition, this->followCamera->position);
		//this->fov = glm::degrees(2 * std::atan(1/(2 * diff)));
		std::cout << "FOV: " << this->fov << std::endl;
		
		this->yaw = this->followCamera->yaw + this->yawOffset;
		std::cout << "Yaw: " << this->yaw << std::endl;
		
		this->pitch = this->followCamera->pitch;

		this->front = glm::vec3(cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
		sin(glm::radians(this->pitch)),
		sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)));
		this->front = glm::normalize(this->front);*/
	}

	/*if (this->followCamera != nullptr) {
		glm::vec3 diff = this->followCamera->position - this->position;
		this->position = this->followCamera->position - glm::normalize(diff) * 3.0f;
	}*/

	if (this->fbt == nullptr) glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else fbt->Bind();
}