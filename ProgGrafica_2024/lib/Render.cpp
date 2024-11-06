#include "Render.h"
#include "InputManager.h"

Render::Render()
{
	Render::r = this;
	this->objectList = std::vector<Object*>();
}

void Render::initGL(const char* windowName, int sizeX, int sizeY)
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "ERROR GLFW no iniciada\n";
		exit(0);
	}

	#ifdef __APPLE__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

	this->window = glfwCreateWindow(sizeX, sizeY, windowName, NULL, NULL);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		exit(-1);
	}
	glfwSetCursorPos(window, sizeX / 2, sizeY / 2);

	InputManager::initInputManager(window);

	glEnable(GL_DEPTH_TEST);
}

void Render::drawObjects()
{
	for (auto& obj : this->objectList)
	{
		drawGL(obj->id);
	}
}

void Render::setupObject(Object* object)
{
	bufferObject_t bo;

	glGenVertexArrays(1, &bo.bufferID);
	glBindVertexArray(bo.bufferID);

	glGenBuffers(1, &bo.vertexBufferID);
	glGenBuffers(1, &bo.edgeBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bo.vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * object->vertexList.size(), object->vertexList.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.edgeBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object->idList.size(), object->idList.data(), GL_STATIC_DRAW);

	bufferObjectList[object->id] = bo;
}

void Render::drawGL(int id)
{
	glm::mat4 M = this->objectList[id]->modelMatrix;
	glm::mat4 V = this->camera->computeViewMatrix();
	glm::mat4 P = this->camera->computeProjectionMatrix();

	glm::mat4 MVP = P * V * M;

	bufferObject_t bo = bufferObjectList[this->objectList[id]->id];
	this->objectList[id]->prg->use();
	glBindVertexArray(bo.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bo.vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.edgeBufferID);

	this->objectList[id]->prg->setMVP(MVP);
	this->objectList[id]->prg->setVertexAttribute("vPos", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexPos));
	this->objectList[id]->prg->setVertexAttribute("vColor", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexColor));
	this->objectList[id]->prg->setVertexAttribute("vNormal", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexNormal));

	glDrawElements(GL_TRIANGLES, this->objectList[id]->idList.size(), GL_UNSIGNED_INT, nullptr);
}

void Render::putObject(Object* object)
{
	this->objectList.push_back(object);
	setupObject(object);
}

void Render::putCamera(Camera* camera)
{
	this->camera = camera;
}

void Render::move(float deltaTime)
{
	for (auto& obj : this->objectList)
	{
		obj->move(deltaTime);
	}
}

void Render::mainLoop() 
{
	double currentTime = 0.0f;
	float lastTime = 0.0f;
	float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(this->window) && !InputManager::keysState[GLFW_KEY_ESCAPE])
	{
		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

		this->camera->move(deltaTime);
		//move(deltaTime);

		drawObjects();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}