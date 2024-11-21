#pragma once

#include "Common.h"

typedef struct
{
	glm::vec4 vertexPos;
	glm::vec4 vertexNormal;
	glm::vec4 vertexColor;
	glm::vec4 vertexUv;
} vertex_t;

typedef struct {
	float Ka, Kd, Ks;
	int shiny;
} material_t;