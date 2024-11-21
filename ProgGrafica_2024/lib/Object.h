#pragma once

#include "Common.h"
#include "vertex.h"
#include "InputManager.h"
#include "RenderProgram.h"
#include "Texture.h"

class Object
{
private:
	static inline int idCounter = 0;
public:
	int id = 0;
	bool activeGravity;

	glm::vec4 position;
	glm::vec4 rotation;
	glm::vec4 scale;
	glm::vec4 velocity;

	Program* prg = new Program();
	material_t material = { 1.0f, 1.0f, 1.0f, 70 };
	Texture* texture = nullptr;

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;

	glm::mat4 modelMatrix;

	Object();

	Object(std::string fileName, bool activeGravity);

	void createTriangle();

	void move(double deltaTime);

	void updateModelMatrix();
};