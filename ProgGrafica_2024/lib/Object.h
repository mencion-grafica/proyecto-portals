#pragma once

#include "Common.h"
#include "vertex.h"
#include "InputManager.h"
#include "RenderProgram.h"
#include "tinyxml2.h"
#include "Joint.h"

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

class Player : public Object
{
private:
	Joint rootJoint;
	int jointCount;

public:
	Player(std::string fileName, glm::vec4 pos);

	void loadDaeFile(const char* fileName);

};