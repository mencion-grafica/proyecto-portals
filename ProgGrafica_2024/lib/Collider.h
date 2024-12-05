#pragma once

#include "Common.h"
#include "vertex.h"

class Collider {
public:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
    std::vector<Collider*> subColliders;

    Collider();
    Collider(glm::vec3 minBounds, glm::vec3 maxBounds);

    void updateBounds(glm::mat4 modelMatrix);

    void subdivide(int levels);

    bool checkCollision(Collider* other);

    bool checkCollisionWithSubColliders(Collider* other);

	void draw(glm::vec3 position);
};