#pragma once

#include "Common.h"

class Joint
{
public:
	std::vector<Joint> children;
	int id;
	glm::mat4 transformationMatrix;
};

