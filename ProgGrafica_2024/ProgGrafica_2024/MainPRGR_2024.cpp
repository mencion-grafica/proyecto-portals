#define GLAD_GL_IMPLEMENTATION
#include "lib/Common.h"

#include "lib/Vector4f.h"
#include "lib/Mat4x4f.h"
#include "lib/Quaternion4f.h"
#include "lib/vertex.h"
#include "lib/Render.h"
#include "lib/InputManager.h"
#include "lib/Object3D.h"
#include "lib/Camera.h"

#define ENEMY 1
#define SPACESHIP 2
#define BULLET_ALLY 3
#define BULLET_ENEMY 4

class cubo3D : public Object3D {

public:
	cubo3D():Object3D("data/cubeBL.trs") {
		
	}

	virtual void move(double timeStep) override {
		if (InputManager::keysState[GLFW_KEY_B]) {
			delete this->tex;
			this->tex = new Texture(512, 512, { 0xFF, 0, 0, 0xFF });
		}
		
		if (InputManager::keysState[GLFW_KEY_V]) {
			delete this->tex;
			this->tex = new Texture(512, 512, { 0xFF, 0xFF, 0, 0xFF });
		}
	}
};

class Enemy : public Object3D {

public:
	Enemy() :Object3D("data/ship.trs") {
		this->rotation.z = 180;
		this->type = ENEMY;
		this->scale = { 0.25f, 0.25f, 0.25f, 0.25f };
		this->tex = new Texture("data/ship.png");
	}

	virtual void move(double timeStep) override {
		//if (Render::r->collisionType(this, BULLET_ALLY)) std::cout << "Collision detected" << std::endl;
		
		this->position.x = (this->position.x > 3) ? -3 : this->position.x + (timeStep * 0.5f);
	}
};

class SpaceShip : public Object3D {

public:
	SpaceShip() :Object3D("data/ship.trs") {
		//this->rotation.z = 180;
		this->type = SPACESHIP;
		this->tex = new Texture("data/ship.png");
		this->scale = { 0.25f, 0.25f, 0.25f, 0.25f };
	}

	virtual void move(double timeStep) override {
		if (InputManager::keysState[GLFW_KEY_SPACE]) 
		{
			//Render::r->putObject(new Bullet(BULLET_ALLY, this->position));
		}

		if (InputManager::keysState[GLFW_KEY_W]) this->position.y += timeStep * 0.5f;
		if (InputManager::keysState[GLFW_KEY_S]) this->position.y -= timeStep * 0.5f;
		if (InputManager::keysState[GLFW_KEY_A]) this->position.x -= timeStep * 0.5f;
		if (InputManager::keysState[GLFW_KEY_D]) this->position.x += timeStep * 0.5f;
	}
};

void nivel0(Render* r)
{
	std::vector<Object3D*> objectList;

	Object3D* g = new Object3D("./data/ground.trs");
	g->scale = { 100.0f, 1.0f, 100.0f, 0.0f };
	objectList.push_back(g);
	
	/**
	// Cubo 3D with different color for each face
	Object3D* cBl = new Object3D("./data/rubikCube.trs");
	objectList.push_back(cBl);
	*/

	// Texture Cube 3D
	Object3D* cBl = new cubo3D();
	cBl->position = { 2, 0, 0, 1 };
	cBl->tex = new Texture(512, 512, { 0, 0, 0xFF, 0xFF });
	cBl->tex = new Texture("./data/top.png");
	objectList.push_back(cBl);
	
	/*
	// Texture Sphere 3D 
	Object3D* s = new Object3D("./data/sphere.trs");
	s->position = { -2, 0, 0, 1 };	
	s->tex = new Texture("./data/earth.png");
	objectList.push_back(s);
	*/

	for (auto& obj : objectList) {
		r->putObject(obj);
	}
}

void nivel1(Render* r)
{
	//ship->position.x = -3;
	std::vector<Object3D*> objectList;
	
	Object3D* ship = new SpaceShip();
	ship->position.y = -1;
	objectList.push_back(ship);

	Object3D* enemy = new Enemy();
	enemy->position.x = -3;
	enemy->position.y = 1;
	objectList.push_back(enemy);

	for (auto& obj : objectList) {
		r->putObject(obj);
	}
}

class Bullet : public Object3D {
public:
	libMath::vector4f direction = { 0, 1, 0, 0 };

	Bullet(int bulletType, libMath::vector4f pos) :Object3D("data/bullet.trs") {
		this->rotation.z = (bulletType == BULLET_ENEMY) ? 180 : 0;
		this->direction = (bulletType == BULLET_ENEMY) ? libMath::vector4f{ 0, -1, 0, 0 } : libMath::vector4f{ 0, 1, 0, 0 };
		this->type = bulletType;
		this->position = pos;
		this->scale = { 0.1f, 0.1f, 0.1f, 0.1f };
		this->tex = new Texture("data/bullet.png");
	}

	virtual void move(double timeStep) override {
		this->position = this->position + (this->direction * timeStep);

		//if (std::abs(this->position.y) > 3) 
	}
};

int main(int argc, char** argv)
{
	Render *render = new Render();
	render->initGL("Practica 3 Camera", 640, 480);

	nivel0(render);
	//nivel1(render);

	// camera fps
	Camera* cam = new Camera({ 0, 0, 3, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 1 }, { 0, 1, 0, 0 }, M_PI / 4.0f, 4.0f / 3.0f, 0.01f, 1000.0f);
	render->putCamera(cam);

	// Crear luces
	Light* l1 = new Light({ 0, 0, 3, 1 }, point);
	l1->setPos({ -10, 0, 0, 1 });
	render->putLight(l1);

	Light* l2 = new Light({ 0, 0, 3, 1 }, point);
	l2->setPos({ 10, 0, 0, 1 });
	l2->setColor({ 0, 1, 0, 1 });
	render->putLight(l2);

	/*
	// Orbital light
	OrbitalLight* l3 = new OrbitalLight(10.0f, 3.0f, { 0, 0, 0, 0});
	l3->setPos({ 0, 10, 0, 1 });
	l3->setColor({ 1, 1, 0, 1 });
	render->putLight(l3);
	*/

	render->mainLoop();

	return 0;
}