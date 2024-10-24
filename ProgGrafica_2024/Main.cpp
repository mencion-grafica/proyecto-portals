#define GLAD_GL_IMPLEMENTATION

#include "lib/Common.h"
#include "lib/Render.h"
#include "lib/Object.h"

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

	for (int i = 0; i < list.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << "[ ";
			for (int k = 0; k < 4; k++) std::cout << list[i][j][k] << " ";
			std::cout << "]" << std::endl;
		}
		std::cout << std::endl;
	}

	Object* object = new Object();
	object->createTriangle();

	Render* render = new Render();
	render->initGL("Proyecto Portal", 1080, 720);

	render->putObject(object);

	render->mainLoop();

	return 0;
}