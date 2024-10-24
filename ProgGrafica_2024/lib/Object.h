#pragma once

#include "Common.h"
#include "vertex.h"
#include "InputManager.h"

class Object
{
	public:
		glm::vec4 position;
		glm::vec4 rotation;
		glm::vec4 scale;

		std::vector<vertex_t> vertexList;
		std::vector<int> idList;

		glm::mat4 modelMatrix;

		Object();

		void createTriangle();

		void move(double deltaTime);

		void updateModelMatrix();
};