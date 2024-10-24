#include "Render.h"
#include "InputManager.h"

Render::Render()
{
	Render::r = this;
}

void Render::initGL(const char* windowName, int sizeX, int sizeY)
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "ERROR GLFW no iniciada\n";
		exit(0);
	}

	this->window = glfwCreateWindow(sizeX, sizeY, windowName, NULL, NULL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0.0, 0.0);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);

	InputManager::initInputManager(window);

	glEnable(GL_DEPTH_TEST);
}

void Render::drawGL()
{
	glBegin(GL_TRIANGLES);

	for (auto i : this->objectList[0]->idList) {
		vertex_t v = this->objectList[0]->vertexList[i];
		glm::vec4 pos = this->objectList[0]->modelMatrix * v.vertexPos;

		glColor3f(v.vertexColor.x, v.vertexColor.y, v.vertexColor.z);
		glVertex3f(pos.x, pos.y, pos.z);
	}

	glEnd();
}

void Render::putObject(Object* object)
{
	this->objectList.push_back(object);
}

void Render::mainLoop() 
{
    while (!glfwWindowShouldClose(this->window) && !InputManager::keysState[GLFW_KEY_ESCAPE])
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
		drawGL();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}