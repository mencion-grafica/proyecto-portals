#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"
#include "lib/Camera.h"
#include "lib/Map.h"
#include "lib/Player.h"

int main(int argc, char** argv)
{
	std::string path = "./data/Map/";
	std::string file = "Prueba.cvmf";
	std::vector<std::string> filenames;

	Render* render = new Render();
	render->initGL("Proyecto Portal", 1200, 800);

	Object* ground = new Object("./data/ground.trs");
	ground->scale = glm::vec4(100.0f, 1.0f, 100.0f, 0.0f);
	ground->initializeCollider();
	ground->alwaysRender = true;
	render->putObject(ground);

	Object* object = new Object("data/cubeBL.trs", true);
	object->position = glm::vec4(0, 20, -10, 1);
	object->initializeCollider();
	render->putObject(object);

	// Player* player = new Player("./data/model.dae", glm::vec4(0, 3, 0, 1));
	// player->initializeCollider();
	// render->putObject(player);

	Map* map = new Map(path + file);
	if (!std::filesystem::exists(path)) {
		std::cout << "No existe el directorio\n";
		return 1;
	}

	for (auto& s : std::filesystem::directory_iterator(path)) {
		if (s.path().extension() == ".trs") {
			filenames.push_back(path + s.path().filename().string());
		}
	}

	Camera* mainCamera = new Camera();
	render->putCamera(mainCamera);

	//We make Portal 1
	Object* portal = new Object("data/plane.trs");
	portal->position = glm::vec4(3, 0, 0, 1);
	portal->scale = glm::vec4(2, 2, 1, 0);
	portal->texture = new FrameBufferTexture(800, 600);
	render->putObject(portal);
	
	Camera* camera = new Camera(glm::vec3(-6.5f, 0.02f, 0.7f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0,1,0), 45, 800.0f/600.0f, portal->texture);
	camera->yaw = 0.0f;
	render->putCamera(camera);


	//blocking box behind portal 2
	Object* blockingObject = new Object("data/cubeBL.trs");
	blockingObject->position = glm::vec4(-8, 0, 0, 1);
	blockingObject->texture = new Texture("data/front.png");
	render->putObject(blockingObject);
	

	Object* portal2 = new Object("data/plane.trs");
	portal2->position = glm::vec4(-6.7f, 0.0f, 1.0f, 1);
	portal2->scale = glm::vec4(2, 2, 1, 0);
	portal2->rotation = glm::vec4(0, 33, 0, 1);
	portal2->texture = new FrameBufferTexture(800, 600);
	render->putObject(portal2);

	Camera* camera2 = new Camera(glm::vec3(3, 0, 0), glm::vec3(3.0f,0.0f,-1.0f), glm::vec3(0,1,0), 45, 800.0f/600.0f, portal2->texture);
	camera2->yaw = 0.0f;
	render->putCamera(camera2);
	

	camera->followCamera = mainCamera;
	camera->portalPosition = portal->position;
	camera->yawOffset = camera->yaw - mainCamera->yaw;
	camera->pivotPosition = glm::vec4(-6.5f, 0.02f, 0.7f, 1.0f);

	camera2->followCamera = mainCamera;
	camera2->portalPosition = portal2->position;
	camera2->yawOffset = camera2->yaw - mainCamera->yaw;
	camera2->pivotPosition = glm::vec4(3, 0, 0, 1);
	
	// Light source
	Light* light = new Light(glm::vec4(0.0f, 20.0f, 0.0f, 1.0f), point);
	light->setScale(glm::vec4(100.0f, 100.0f, 100.0f, 0.0f));
	light->setPosition(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	light->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	render->putLight(light);
	
	// Necesario para que el render no se cague encima ¯\_(ツ)_/¯
	Object* triangle = new Object();
	triangle->prg->addShader("data/shader.vert");
	triangle->prg->addShader("data/shader.frag");
	triangle->prg->link();
	render->putObject(triangle);

	render->renderColliders = true;
	render->mainLoop();
	
	return 0;
}