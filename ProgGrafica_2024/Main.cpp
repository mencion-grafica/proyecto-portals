#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"
#include "lib/Camera.h"

glm::mat4 transform(glm::vec2 const& Orientation, glm::vec3 const& Translate, glm::vec3 const& Up)
{
    glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 10.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
    glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
    glm::mat4 Model = glm::mat4(1.0f);
    return Proj * View * Model;
}

glm::mat4 computeModelViewMatrix(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 View = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5f));
	return View * Model;
}

int main(int argc, char** argv)
{
	glm::mat4 newMat = transform(glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 newMat2 = computeModelViewMatrix(5.0f, glm::vec2(0.0f, 0.0f));
	std::vector<glm::mat4> list = { newMat, newMat2 };

	Render* render = new Render();
	render->initGL("Proyecto Portal", 1080, 720);

	Object* object = new Object("./data/cube.trs");
	object->position = glm::vec4(0, 0, 0, 1);

	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), M_PI / 4.0f, 4.0f / 3.0f, 0.01f, 1000.0f);

	render->putCamera(camera);
	render->putObject(object);

	render->mainLoop();

	return 0;
}