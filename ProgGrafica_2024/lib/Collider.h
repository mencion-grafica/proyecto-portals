#pragma once

#include "Common.h"
#include "vertex.h"

class Collider {
public:
	int id;
	glm::vec3 minBounds;
	glm::vec3 maxBounds;
	
	Collider();

	bool checkCollision(Collider* other);
	bool checkCollision(glm::vec4 position);
	void computeBounds(const glm::mat4& modelMatrix, const std::vector<vertex_t>& vertexList);

	void draw();
};