#pragma once

#include "Common.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class Joint
{
public:
	static inline int idCounter = 0;

	std::vector<Joint> children;
	int id;
	glm::mat4 transformationMatrix;

	Joint();

	Joint(const char* fileName);

	Joint(int id, XMLElement* joint);

	int GetIdCounter();
};

