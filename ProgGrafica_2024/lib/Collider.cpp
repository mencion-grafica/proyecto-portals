#include "Collider.h"

#include "Camera.h"
#include "Camera.h"

Collider::Collider() : minBounds(glm::vec3(0.0f)), maxBounds(glm::vec3(0.0f)) {}

Collider::Collider(glm::vec3 minBounds, glm::vec3 maxBounds) : minBounds(minBounds), maxBounds(maxBounds) {}

void Collider::updateBounds(glm::mat4 modelMatrix)
{
    glm::vec4 corners[8] = {
        glm::vec4(minBounds.x, minBounds.y, minBounds.z, 1.0f),
        glm::vec4(maxBounds.x, minBounds.y, minBounds.z, 1.0f),
        glm::vec4(minBounds.x, maxBounds.y, minBounds.z, 1.0f),
        glm::vec4(maxBounds.x, maxBounds.y, minBounds.z, 1.0f),
        glm::vec4(minBounds.x, minBounds.y, maxBounds.z, 1.0f),
        glm::vec4(maxBounds.x, minBounds.y, maxBounds.z, 1.0f),
        glm::vec4(minBounds.x, maxBounds.y, maxBounds.z, 1.0f),
        glm::vec4(maxBounds.x, maxBounds.y, maxBounds.z, 1.0f)
    };

    glm::vec3 newMin = glm::vec3(FLT_MAX);
    glm::vec3 newMax = glm::vec3(-FLT_MAX);

    for (int i = 0; i < 8; i++) {
        glm::vec3 transformedCorner = glm::vec3(modelMatrix * corners[i]);
        newMin = glm::min(newMin, transformedCorner);
        newMax = glm::max(newMax, transformedCorner);
    }

    minBounds = newMin;
    maxBounds = newMax;

    // Update sub-colliders recursively
    for (auto& subCollider : subColliders) {
        subCollider->updateBounds(modelMatrix);
    }
}

void Collider::subdivide(int levels) 
{
    if (levels <= 0) return;

    glm::vec3 center = (minBounds + maxBounds) * 0.5f;

    // Create 8 smaller bounding boxes (octree-like subdivision)
    subColliders.clear();
    subColliders.push_back(new Collider(minBounds, center));
    subColliders.push_back(new Collider(glm::vec3(center.x, minBounds.y, minBounds.z), glm::vec3(maxBounds.x, center.y, center.z)));
    subColliders.push_back(new Collider(glm::vec3(minBounds.x, center.y, minBounds.z), glm::vec3(center.x, maxBounds.y, center.z)));
    subColliders.push_back(new Collider(glm::vec3(center.x, center.y, minBounds.z), glm::vec3(maxBounds.x, maxBounds.y, center.z)));
    subColliders.push_back(new Collider(glm::vec3(minBounds.x, minBounds.y, center.z), glm::vec3(center.x, center.y, maxBounds.z)));
    subColliders.push_back(new Collider(glm::vec3(center.x, minBounds.y, center.z), glm::vec3(maxBounds.x, center.y, maxBounds.z)));
    subColliders.push_back(new Collider(glm::vec3(minBounds.x, center.y, center.z), glm::vec3(center.x, maxBounds.y, maxBounds.z)));
    subColliders.push_back(new Collider(center, maxBounds));

    // Recursively subdivide each sub-collider
    for (auto& subCollider : subColliders) 
    {
        subCollider->subdivide(levels - 1);
    }
}

std::string vec3_to_string(glm::vec3 v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

bool Collider::checkCollision(Collider* other) 
{
    std::cout << "Checking collision between " << vec3_to_string(minBounds) << " and " << vec3_to_string(other->minBounds) << std::endl;
    return (minBounds.x <= other->maxBounds.x && maxBounds.x >= other->minBounds.x) &&
        (minBounds.y <= other->maxBounds.y && maxBounds.y >= other->minBounds.y) &&
        (minBounds.z <= other->maxBounds.z && maxBounds.z >= other->minBounds.z);
}

bool Collider::checkCollisionWithSubColliders(Collider* other) 
{
    // First, check the parent collider
    if (!checkCollision(other)) return false;

    // Check sub-colliders for a more refined collision
    for (const auto& sub_collider : subColliders) 
    {
        if (sub_collider->checkCollision(other)) return true;
    }

    return false;
}

void drawWireframeBox(glm::vec3 position, glm::vec3 minBounds, glm::vec3 maxBounds) 
{
    // Define the vertices of the box
    glm::vec3 vertices[8] = {
        minBounds,
        glm::vec3(maxBounds.x, minBounds.y, minBounds.z),
        glm::vec3(minBounds.x, maxBounds.y, minBounds.z),
        glm::vec3(maxBounds.x, maxBounds.y, minBounds.z),
        glm::vec3(minBounds.x, minBounds.y, maxBounds.z),
        glm::vec3(maxBounds.x, minBounds.y, maxBounds.z),
        glm::vec3(minBounds.x, maxBounds.y, maxBounds.z),
        maxBounds
    };

    // Define the edges of the box
    unsigned int edges[24] = {
        0, 1, 1, 3, 3, 2, 2, 0, // Bottom face
        4, 5, 5, 7, 7, 6, 6, 4, // Top face
        0, 4, 1, 5, 3, 7, 2, 6  // Vertical edges
    };

    // Render the edges using GL_LINES
    glBegin(GL_LINES);
    for (const auto& edge : edges)
    {
        glVertex3f(vertices[edge].x, vertices[edge].y, vertices[edge].z);
    }
    glEnd();
}


void Collider::draw(glm::vec3 position)
{
	drawWireframeBox(position, minBounds, maxBounds);

	for (auto& subCollider : subColliders)
	{
		subCollider->draw(position);
	}
}