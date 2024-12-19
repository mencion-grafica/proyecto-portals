#pragma once

#include "Common.h"
#include "vertex.h"
#include "InputManager.h"
#include "RenderProgram.h"
#include "tinyxml2.h"
#include "Joint.h"

using namespace tinyxml2;
#include "Texture.h"

class Object
{
protected:
	static inline int idCounter = 0;
public:
	int id = 0;
	bool activeGravity;

	glm::vec4 position;
	glm::vec4 rotation;
	glm::vec4 scale;
	glm::vec4 velocity = glm::vec4(0.0f);
	float gravityForce = -9.8f;
	bool gravity;

	Program* prg = new Program();
	material_t material = { 1.0f, 1.0f, 1.0f, 70 };
	Texture* texture = nullptr;

	std::vector<vertex_t> vertexList;
	std::vector<int> idList;

	glm::mat4 modelMatrix;

	Object();

	Object(std::string fileName, bool gravity);

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