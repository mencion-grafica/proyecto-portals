#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"
#include "lib/Camera.h"

int main(int argc, char** argv)
{
	Render* render = new Render();
	render->initGL("Proyecto Portal", 800, 600);

	Object* ground = new Object("data/ground.trs");
	ground->scale = glm::vec4(100.0f, 1.0f, 100.0f, 0.0f);
	ground->alwaysRender = true;
	render->putObject(ground);

	Object* object = new Object("data/cubeBL.trs");
	object->position = glm::vec4(0, 0, 0, 1);
	object->texture = new Texture("data/front.png");
	render->putObject(object);

	Player* player = new Player("./data/player.dae", glm::vec4(0, 0, 0, 1));
	render->putObject(player);

	Object* portal = new Object("data/plane.trs");
	portal->position = glm::vec4(3, 0, 0, 1);
	portal->scale = glm::vec4(2, 2, 1, 0);
	portal->texture = new FrameBufferTexture(800, 600);
	render->putObject(portal);

	Camera* camera = new Camera(glm::vec3(-6.5f, 0.02f, 0.7f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0,1,0), 45, 800.0f/600.0f, portal->texture);
	camera->yaw = 0.0f;
	render->putCamera(camera);

	Object* blockingObject = new Object("data/cubeBL.trs");
	blockingObject->position = glm::vec4(-8, 0, 0, 1);
	blockingObject->texture = new Texture("data/front.png");
	render->putObject(blockingObject);
	
	Camera* camera2 = new Camera();
	render->putCamera(camera2);

	camera->followCamera = camera2;
	camera->portalPosition = portal->position;
	camera->yawOffset = camera->yaw - camera2->yaw;
	camera->pivotPosition = glm::vec4(-6.5f, 0.02f, 0.7f, 1.0f);
	
	render->mainLoop();

	return 0;
}