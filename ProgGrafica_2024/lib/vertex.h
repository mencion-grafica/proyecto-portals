#pragma once

#include "Common.h"

typedef struct
{
	glm::vec4 vertexPos;
	glm::vec4 vertexNormal;
	glm::vec4 vertexColor;
	glm::ivec3 idJoints;
	glm::vec3 weightJoints;
} vertex_t;