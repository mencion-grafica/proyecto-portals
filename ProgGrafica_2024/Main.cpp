#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"
#include "lib/Camera.h"

int main(int argc, char** argv)
{
	Render* render = new Render();
	render->initGL("Proyecto Portal", 800, 600);

	Object* ground = new Object("data/ground.trs", false);
	ground->scale = glm::vec4(100.0f, 1.0f, 100.0f, 0.0f);
	render->putObject(ground);

	Object* object = new Object("data/cubeBL.trs", true);
	object->position = glm::vec4(6, 20, 0, 1);
	object->texture = new Texture("data/front.png");
	object->createCamera();
	render->putObject(object);

	//Camera* camera = new Camera();
	render->putCamera(object->camera);
	
	render->mainLoop();

	return 0;
}