#include "Collider.h"

#include <algorithm>

#include "Camera.h"
#include "Camera.h"

std::string vec3_to_string(glm::vec3 v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

void drawWireframeBox(glm::vec3 minBounds, glm::vec3 maxBounds) 
{
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

    unsigned int edges[24] = {
        0, 1, 1, 3, 3, 2, 2, 0, 
        4, 5, 5, 7, 7, 6, 6, 4, 
        0, 4, 1, 5, 3, 7, 2, 6 
    };

    glBegin(GL_LINES);
    for (const auto& edge : edges) glVertex3f(vertices[edge].x, vertices[edge].y, vertices[edge].z);
    glEnd();
}

Collider::Collider()
{
    this->minBounds = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    this->maxBounds = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void Collider::computeBounds(const glm::mat4& modelMatrix, const std::vector<vertex_t>& vertexList)
{
    glm::vec3 checkMin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 checkMax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    
    for (const auto& vertex : vertexList)
    {
        glm::vec4 transformedVertex = modelMatrix * vertex.vertexPos;
        checkMin = glm::min(checkMin, glm::vec3(transformedVertex));
        checkMax = glm::max(checkMax, glm::vec3(transformedVertex));
    }

    this->minBounds = checkMin;
    this->maxBounds = checkMax;
}

bool Collider::checkCollision(Collider* other)
{
    return (this->minBounds.x <= other->maxBounds.x && this->maxBounds.x >= other->minBounds.x) &&
           (this->minBounds.y <= other->maxBounds.y && this->maxBounds.y >= other->minBounds.y) &&
           (this->minBounds.z <= other->maxBounds.z && this->maxBounds.z >= other->minBounds.z);
}

bool Collider::checkCollision(glm::vec4 position)
{
    return (this->minBounds.x <= position.x && this->maxBounds.x >= position.x) &&
           (this->minBounds.y <= position.y && this->maxBounds.y >= position.y) &&
           (this->minBounds.z <= position.z && this->maxBounds.z >= position.z);
}

void Collider::draw()
{
	drawWireframeBox(this->minBounds, this->maxBounds);
}