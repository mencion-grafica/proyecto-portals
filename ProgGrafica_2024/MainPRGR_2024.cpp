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
#include "lib/CameraFPS.h"

class cubo3D : public Object3D {

public:
	cubo3D():Object3D("data/cubeBL.trs", {0, 0, 0, 1}) {
		
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

void nivel0(Render* r)
{
	r->initGL("Proyecto Portals", 1080, 720);

	std::vector<Object3D*> objectList;

	// Camera FPS
	Camera* cam = new CameraFPS(5.0f, { 0, 0, 3, 1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 1 }, M_PI / 4.0f, 4.0f / 3.0f, 0.01f, 100.0f);
	r->putCamera(cam);

	Object3D* g = new Object3D("./data/ground.trs", { 0, 0, 0, 1 });
	g->scale = { 100.0f, 1.0f, 100.0f, 0.0f };
	objectList.push_back(g);

	// Texture Sphere 3D 
	Object3D* s = new Object3D("./data/sphere.trs", { 0, 2, 0, 1 });
	s->createSphereCollider();
	s->type = SPHERE;
	objectList.push_back(s);

	Object3D* mercury = new Object3D("./data/sphere.trs", { 6, 2, 0, 1 });
	mercury->createSphereCollider();
	mercury->type = SPHERE;
	objectList.push_back(mercury);

	// Sun
	Object3D* sun = new Object3D("./data/sphere.trs", { 12, 2, 0, 1 });
	sun->createSphereCollider();
	sun->type = SPHERE;
	objectList.push_back(sun);

	Light* l1 = new Light({ 0, 0, 3, 1 }, point);
	l1->setPos({ -10, 0, 0, 1 });
	r->putLight(l1);

	Light* l2 = new Light({ 0, 0, 3, 1 }, point);
	l2->setPos({ 10, 0, 0, 1 });
	l2->setColor({ 0, 0, 1, 1 });
	r->putLight(l2);

	OrbitalLight* l3 = new OrbitalLight({0, 0, 3, 1}, 10.0f, 1.0f);
	l3->setPos({ 0, 10, 0, 1 });
	l3->setColor({ 0, 1, 0, 1 });
	r->putLight(l3);

	for (auto& obj : objectList) {
		r->putObject(obj);
	}
}

int main(int argc, char** argv)
{
	Render *render = new Render();

	nivel0(render);

	render->mainLoop();

	return 0;
}