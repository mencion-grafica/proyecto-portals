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
	//Matriz de transformaci�n global
	glm::mat4 transformationMatrix;
	//Matriz de transformaci�n respecto al padre
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

