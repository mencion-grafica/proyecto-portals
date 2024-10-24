#include "Object.h"

Object::Object()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->modelMatrix = glm::identity<glm::mat4>();
}

void Object::createTriangle()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->vertexList = 
	{
		{
			{0, 0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{1, 0, 0, 1} // Color
		},
		{
			{-0.5f, -0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 1, 0, 1} // Color
		},
		{
			{0.5f, -0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 0, 1, 1} // Color
		}
	};

	this->idList = { 1, 0, 2 };
	this->modelMatrix = glm::identity<glm::mat4>();
}

void Object::move(double deltaTime)
{
	if (InputManager::keysState[GLFW_KEY_A]) position.x -= 0.2f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_D]) position.x += 0.2f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_W]) position.y += 0.2f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_S]) position.y -= 0.2f * deltaTime;

	if (InputManager::keysState[GLFW_KEY_Q]) rotation.y += 0.8f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_E]) rotation.y -= 0.8f * deltaTime;

	if (InputManager::keysState[GLFW_KEY_Z]) scale.x -= 0.1f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_Z]) scale.y -= 0.1f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_Z]) scale.z -= 0.1f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_X]) scale.x += 0.1f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_X]) scale.y += 0.1f * deltaTime;
	if (InputManager::keysState[GLFW_KEY_X]) scale.z += 0.1f * deltaTime;

	updateModelMatrix();
}

void Object::updateModelMatrix()
{
	glm::mat4 translate = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position));
	glm::mat4 rotate = glm::rotate(glm::identity<glm::mat4>(), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(this->scale));

	this->modelMatrix = translate * rotate * scale;
}