#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"
#include "lib/Camera.h"

int main(int argc, char** argv)
{
	Render* render = new Render();
	render->initGL("Proyecto Portal", 1200, 800);

	Object* ground = new Object("data/ground.trs");
	ground->scale = glm::vec4(100.0f, 1.0f, 100.0f, 0.0f);
	ground->initializeCollider();
	render->putObject(ground);

	Object* object = new Object("data/cubeBL.trs");
	object->position = glm::vec4(0, 3, -10, 1);
	object->initializeCollider();
	render->putObject(object);

	Player* player = new Player("./data/player.dae", glm::vec4(0, 3, 0, 1));
	player->initializeCollider();
	render->putObject(player);

	// Necesario para que el render no se cague encima ¯\_(ツ)_/¯
	Object* triangle = new Object();
	render->putObject(triangle);
	
	Camera* camera = new Camera();
	render->putCamera(camera);

	render->renderColliders = true;
	render->mainLoop();

	return 0;
}