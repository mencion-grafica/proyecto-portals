#include "Render.h"
#include "InputManager.h"

Render::Render()
{
	Render::r = this;
	this->objectList = std::vector<Object*>();
	this->cameraList = std::list<Camera*>();
	this->cameraList.push_back(new Camera());
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(*Render::r->cameraList.begin())->aspectRatio = (float) width / (float) height;
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	Camera* cam = (*Render::r->cameraList.begin());

	if (cam->firstMouse)
	{
		cam->lastX = xPos;
		cam->lastY = yPos;
		cam->firstMouse = false;
	}

	float xOffset = xPos - cam->lastX;
	float yOffset = cam->lastY - yPos;
	cam->lastX = xPos;
	cam->lastY = yPos;

	float sensitivity = 0.2f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	cam->yaw += xOffset;
	cam->pitch += yOffset;

	if (cam->pitch > 89.0f) cam->pitch = 89.0f;
	if (cam->pitch < -89.0f) cam->pitch = -89.0f;

	glm::vec3 front = glm::vec3(
		cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch)),
		sin(glm::radians(cam->pitch)),
		sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch))
	);
	cam->front = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	(*Render::r->cameraList.begin())->fov -= (float) yOffset;
	if ((*Render::r->cameraList.begin())->fov < 1.0f) (*Render::r->cameraList.begin())->fov = 1.0f;
	if ((*Render::r->cameraList.begin())->fov > 45.0f) (*Render::r->cameraList.begin())->fov = 45.0f;
}

void Render::initGL(const char* windowName, int sizeX, int sizeY)
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "ERROR GLFW no iniciada\n";
		exit(0);
	}

	this->window = glfwCreateWindow(sizeX, sizeY, windowName, NULL, NULL);
	this->width = sizeX;
	this->height = sizeY;

	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0)
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		exit(-1);
	}

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
	glm::mat4 V = (*Render::r->cameraList.begin())->computeViewMatrix();
	glm::mat4 P = (*Render::r->cameraList.begin())->computeProjectionMatrix();

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
	(*Render::r->cameraList.begin()) = camera;
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

		

    	move(deltaTime);

    	for(int i=cameraList.size()-1;i>=0;i--)
    	{
    		auto cam=(*Render::r->cameraList.begin()+i);
    	
    		cam->move(deltaTime);

    		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    		
    		drawObjects();
    	}
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}