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
	//Matriz de transformación global
	glm::mat4 transformationMatrix;
	//Matriz de transformación respecto al padre
	glm::mat4 localBindTransform;
	glm::mat4 inverseBindTransform;

	Joint();

	Joint(const char* fileName);

	Joint(int id, XMLElement* joint);

	int GetIdCounter();

	void CalcInverseBindTransform(glm::mat4 parentBindTransform);

	void SetTransformMatrix(glm::mat4 matrix);

	Joint GetJointById(int id);
};

