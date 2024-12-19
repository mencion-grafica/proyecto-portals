#include "Camera.h"

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
	if (this->position.y <= 0.1f) {  // Asegura que solo puede saltar si está cerca del suelo
		this->velocity.y = 5.0f;  // Velocidad inicial del salto
	}
}

void Camera::portal()
{

}

void Camera::move(float deltaTime)
{
	float speed = 1.5f * deltaTime;
	const float gravity = 9.8f;
	if (x == NULL)
		x = 800;

	if (InputManager::keysState[GLFW_KEY_Q]) {
		portal();
	}

	if (InputManager::keysState[GLFW_KEY_SPACE]) {
		jump();
	}

	this->velocity.y += gravity * deltaTime;  // Integra la gravedad
	this->position += this->velocity * deltaTime;  // Mueve la cámara basándose en la velocidad actual

	// Comprobar si la cámara ha "golpeado el suelo"
	if (this->position.y < 0) {
		this->position.y = 0;
		this->velocity.y = 0;
	}

	/*if (InputManager::keysState[GLFW_KEY_W]) this->position += speed * this->front;
	if (InputManager::keysState[GLFW_KEY_S]) this->position -= speed * this->front;
	if (InputManager::keysState[GLFW_KEY_A]) this->position -= glm::normalize(glm::cross(this->front, this->up)) * speed;
	if (InputManager::keysState[GLFW_KEY_D]) this->position += glm::normalize(glm::cross(this->front, this->up)) * speed;*/
	if (InputManager::keysState[GLFW_KEY_W]) this->position += glm::vec3(this->front.x, 0, this->front.z) * speed;
	if (InputManager::keysState[GLFW_KEY_S]) this->position -= glm::vec3(this->front.x, 0, this->front.z) * speed;
	if (InputManager::keysState[GLFW_KEY_A]) this->position -= glm::normalize(glm::cross(this->front, this->up)) * speed;
	if (InputManager::keysState[GLFW_KEY_D]) this->position += glm::normalize(glm::cross(this->front, this->up)) * speed;

	if (InputManager::keysState[GLFW_KEY_E]) this->position += speed * this->up;
	if (InputManager::keysState[GLFW_KEY_Q]) this->position -= speed * this->up;
	if (this->position.y <= 0)
		if (InputManager::keysState[GLFW_KEY_R])
			x = 799;
	if(x != 800){
		x--;
		this->position += x * 0.01f * speed * this->up;
	}
		
	if (x == 0)
		x = 800;
	


	//this->velocity.y += gravity * deltaTime;
	//if (this->position.y <= 0)
		//if (this->activeGravity) 
		//this->position += this->up;
	//else
		//this->position += gravity * this->up;
	if (this->position.y >= 0) {
		this->position -= speed * aire * this->up;
		if (iteraciones >= 130) {
			aire = aire * 1.2f;
			iteraciones = 0;
		}
			
	}
	else
		aire = 1;
	iteraciones++;
}