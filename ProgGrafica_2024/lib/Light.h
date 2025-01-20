#pragma once

#include "Common.h"
#include "InputManager.h"

typedef enum {
    point = 0,
    directional = 1
} lightType;

class Light
{
public:
    glm::vec4 position = glm::vec4(0.0f);
    glm::vec4 rotation = glm::vec4(0.0f);
    glm::vec4 scale = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

    float Ia = 0.2f, Id = 1.0f, Is = 1;
    int enable = 1;

    glm::vec4 color = glm::vec4(1.0f);
    lightType type = point;
    glm::vec4 direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

    Light(glm::vec4 position = glm::vec4(0.0f),
        lightType type = point,
        glm::vec4 direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f))
    : position(position), type(type), direction(direction), color(color) {}

    void setDirection(glm::vec4 direction);
    void setScale(glm::vec4 scale);
    void setColor(glm::vec4 color);
    void setPosition(glm::vec4 position);

    virtual void move(float deltaTime)
    {
        float radius = 3.0f;
        float speed = 2.0f;
        float angle = speed * deltaTime;
        
        this->rotation.y += angle;
        this->position.x = radius * cos(this->rotation.y);
        this->position.z = radius * sin(this->rotation.y);
    }
};
