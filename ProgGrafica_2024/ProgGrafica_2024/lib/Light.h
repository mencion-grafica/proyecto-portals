#pragma once

#include "Common.h"
#include "Vector4f.h"
#include "Mat4x4f.h"
#include "InputManager.h"

typedef enum {
	point = 0,
	directional = 1
} lightType;

class Light
{
public:
	libMath::vector4f pos = { 0, 0, 0, 0 };
	libMath::vector4f rot = { 0, 0, 0, 0 };
	libMath::vector4f scale = { 1, 1, 1, 0 };

	float Ia = 0.2f, Id = 1, Is = 1;
	int enable = 1;

	libMath::vector4f color = { 1, 1, 1, 1 };
	lightType type = point;
	libMath::vector4f direction = { 0, -1, 0, 0 };

	Light(libMath::vector4f pos = { 0, 0, 0, 0 }, lightType type = point, libMath::vector4f direction = { 0, -1, 0, 0 }, libMath::vector4f color = { 1, 1, 1, 0 }) 
		: pos(pos), type(type), direction(direction), color(color) {}

	void setDirection(libMath::vector4f direction) {
		this->direction = direction;
	}

	void setColor(libMath::vector4f color) {
		this->color = color;
	}

	void setPos(libMath::vector4f pos) {
		this->pos = pos;
	}

	virtual void move(float deltaTime) {

	}
};

class OrbitalLight : public Light {
private:
	float radius;
	float speed;
	float angle = 0;
	libMath::vector4f orbitCenter;

public:
	OrbitalLight(float distance, float orbitSpeed, const libMath::vector4f& center)
		: radius(distance), speed(orbitSpeed), orbitCenter(center) {}

	void move(float deltaTime) override {
		angle += libMath::degreesToRadians(speed * deltaTime);

		float x = radius * cos(angle);
		float z = radius * sin(angle);

		libMath::mat4x4f rotationMatrix = libMath::rotY(angle);

		libMath::mat4x4f translationMatrix = libMath::make_translate(orbitCenter.x, orbitCenter.y, orbitCenter.z);
		libMath::mat4x4f transformationMatrix = translationMatrix * rotationMatrix;

		libMath::vector4f temp = { x, 0, z, 1 };
		libMath::vector4f newPosition = transformationMatrix * temp;

		setPos({ newPosition.x, newPosition.y, newPosition.z, 1 });
	}
};
