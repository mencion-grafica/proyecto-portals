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
	if (this->position.y <= 0.1f || canJump) {
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
	float speed = 50.0f * deltaTime;
    if(this->followCamera == nullptr){
	    if (InputManager::keysState[GLFW_KEY_Q]) {
		    portal();
	    }

	    if (InputManager::keysState[GLFW_KEY_SPACE]) {
		    jump();
	    }
    
	    this->velocity.y += gravity * deltaTime;

	    /*if (InputManager::keysState[GLFW_KEY_W]) this->position += speed * this->front;
	    if (InputManager::keysState[GLFW_KEY_S]) this->position -= speed * this->front;
	    if (InputManager::keysState[GLFW_KEY_A]) this->position -= glm::normalize(glm::cross(this->front, this->up)) * speed;
	    if (InputManager::keysState[GLFW_KEY_D]) this->position += glm::normalize(glm::cross(this->front, this->up)) * speed;*/
	    if (InputManager::keysState[GLFW_KEY_W]) this->position += glm::vec3(this->front.x, 0, this->front.z) * speed;
	    if (InputManager::keysState[GLFW_KEY_S]) this->position -= glm::vec3(this->front.x, 0, this->front.z) * speed;
	    if (InputManager::keysState[GLFW_KEY_A]) this->position -= glm::normalize(glm::cross(this->front, this->up)) * speed;
	    if (InputManager::keysState[GLFW_KEY_D]) this->position += glm::normalize(glm::cross(this->front, this->up)) * speed;

	    if (!Render::r->checkCollisions(glm::vec4(this->position, 1.0f))) this->position += this->velocity * deltaTime;
	    else canJump = true;
	    if (InputManager::keysState[GLFW_KEY_E]) this->position += speed * this->up;
	    if (InputManager::keysState[GLFW_KEY_Q]) this->position -= speed * this->up;

	    // Comprobar si la c�mara ha "golpeado el suelo"
	    if (this->position.y < 0) {
		    this->position.y = 0;
		    this->velocity.y = 0;
	    }
    } else {
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
    }
    if (this->fbt == nullptr) glBindFramebuffer(GL_FRAMEBUFFER, 0);
    	else fbt->Bind();
}