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
	render->putObject(ground);

	Object* object = new Object("data/cubeBL.trs");
	object->position = glm::vec4(0, 3, -10, 1);
	object->initializeCollider();
	render->putObject(object);

	Player* player = new Player("./data/model.dae", glm::vec4(0, 3, 0, 1));
	player->initializeCollider();
	render->putObject(player);


	Map* map = new Map(path+file);

	if (!std::filesystem::exists(path)) {
		std::cout << "No existe el directorio" << std::endl;
		return 1;
	}

	for (auto& s : std::filesystem::directory_iterator(path)) {
		if (s.path().extension() == ".trs") {
			filenames.push_back(path + s.path().filename().string());
		}
	}

	/*for (int i = 0; i < 297; i++) {
		Object* object = new Object(filenames[i]);
		//object->scale = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		render->putObject(object);
	}*/

	for (auto& f : filenames) {
		std::cout << "Leyendo archivo: " + f << std::endl;
		Object* object = new Object(f);
		object->scale = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		object->initializeCollider();
		render->putObject(object);
	}
	
	/*Player* player = new Player("./data/model.dae");
	player->position = glm::vec4(0, 0, 0, 1);
	render->putObject(player);*/

	// Necesario para que el render no se cague encima ¯\_(ツ)_/¯
	Object* triangle = new Object();
	render->putObject(triangle);
	triangle->prg->addShader("data/shader.vert");
	triangle->prg->addShader("data/shader.frag");
	triangle->prg->link();
	
	Camera* camera = new Camera();
	camera->position = glm::vec3(0, 0, 0);
	render->putCamera(camera);

	render->renderColliders = true;
	render->mainLoop();
	
	return 0;
}