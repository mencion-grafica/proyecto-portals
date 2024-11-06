#pragma once

#include "Common.h"
#include "vertex.h"
#include "InputManager.h"
#include "RenderProgram.h"

class Object
{
private:
	static inline int idCounter = 0;
public:
	int id = 0;

	glm::vec4 position;
	glm::vec4 rotation;
	glm::vec4 scale;

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;

	glm::mat4 modelMatrix;

	Program* prg = new Program();

	Object();

	Object(std::string fileName);

	void createTriangle();

	void move(double deltaTime);

	void updateModelMatrix();
};