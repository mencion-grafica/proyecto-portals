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
	render->initGL("Proyecto Portal", 800, 600);

	/*Object* ground = new Object("./data/ground.trs");
	ground->scale = glm::vec4(100.0f, 1.0f, 100.0f, 0.0f);
	render->putObject(ground);*/

	/*Object* object = new Object("./data/cube.trs");
	object->position = glm::vec4(0, 0, 0, 1);
	render->putObject(object);*/


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

	for (auto& f : filenames) {
		std::cout << "Leyendo archivo: " + f << std::endl;
		Object* object = new Object(f);
		object->scale = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		render->putObject(object);
	}
	
	/*Player* player = new Player("./data/model.dae");
	player->position = glm::vec4(0, 0, 0, 1);
	render->putObject(player);*/

	Camera* camera = new Camera();
	camera->position = glm::vec3(0, 0, 0);
	render->putCamera(camera);

	render->mainLoop();

	return 0;
}