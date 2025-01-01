#pragma once

#include "Common.h"
#include "vertex.h"
#include "InputManager.h"
#include "RenderProgram.h"
#include "tinyxml2.h"
#include "Joint.h"
#include "Texture.h"
#include "Animator.h"

using namespace tinyxml2;

class Object
{
protected:
	static inline int idCounter = 0;
public:
	int id = 0;

	glm::vec4 position;
	glm::vec4 rotation;
	glm::vec4 scale;

	Program* prg = new Program();
	material_t material = { 1.0f, 1.0f, 1.0f, 70 };
	Texture* texture = nullptr;

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;

	glm::mat4 modelMatrix;

	Object();

	Object(std::string fileName);

	void createTriangle();

	virtual void move(double deltaTime);

	void updateModelMatrix();
};