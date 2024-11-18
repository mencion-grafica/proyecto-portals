#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"
#include "lib/Camera.h"

int main(int argc, char** argv)
{
	Render* render = new Render();
	render->initGL("Proyecto Portal", 800, 600);

	Object* ground = new Object("./data/ground.trs");
	ground->scale = glm::vec4(100.0f, 1.0f, 100.0f, 0.0f);
	render->putObject(ground);

	Object* object = new Object("./data/cube.trs");
	object->position = glm::vec4(0, 0, 0, 1);
	render->putObject(object);

	Player* player = new Player("./data/triangulated.dae", glm::vec4(0, 0, 0, 1));
	render->putObject(player);

	Camera* camera = new Camera();
	render->putCamera(camera);
	
	render->mainLoop();

	return 0;
}