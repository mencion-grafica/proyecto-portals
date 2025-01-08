#include "Render.h"
#include "InputManager.h"
#include "Player.h";

const int MAX_JOINTS = 50;

Render::Render()
{
	Render::r = this;
	this->objectList = std::vector<Object*>();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Render::r->camera->aspectRatio = (float) width / (float) height;
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	Camera* cam = Render::r->camera;

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
	Render::r->camera->fov -= (float) yOffset;
	if (Render::r->camera->fov < 1.0f) Render::r->camera->fov = 1.0f;
	if (Render::r->camera->fov > 45.0f) Render::r->camera->fov = 45.0f;
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
	//glEnable(GL_CULL_FACE);
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
	this->objectList[id]->prg->setMatrix("M", M);
	//for para array de transforms del shader
	if (typeid(*this->objectList[id]) == typeid(Player)) {
		Player* player = dynamic_cast<Player*> (this->objectList[id]);

		//player->UpdateVertex();

		std::vector<glm::mat4> list = player->GetJointTransforms();

		GLint jointTransformsLocation = glGetUniformLocation(this->objectList[id]->prg->programID, "jointTransforms");
		glUniformMatrix4fv(jointTransformsLocation, list.size(), GL_FALSE, &list[0][0][0]);

		/*for (int i = 0; i < list.size(); i++) {
			this->objectList[id]->prg->setMatrix("jointTransforms[" + std::to_string(i) + "]", list[i]);
		}*/
	}
	this->objectList[id]->prg->setVertexAttribute("vPos", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexPos));
	this->objectList[id]->prg->setVertexAttribute("vColor", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexColor));
	this->objectList[id]->prg->setVertexAttribute("vNormal", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexNormal));
	this->objectList[id]->prg->setVertexAttribute("vUv", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexUv));
	this->objectList[id]->prg->setVertexAttribute("jointIndex", 4, GL_INT, sizeof(vertex_t), (void*)offsetof(vertex_t, idJoints));
	this->objectList[id]->prg->setVertexAttribute("weightJoints", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, weightJoints));

	this->objectList[id]->prg->setInteger("textureColor", 0);
	if (this->objectList[id]->texture != nullptr) 
	{
		this->objectList[id]->prg->setInteger("material.textureEnabled", 1);
		this->objectList[id]->texture->bind(0);
	}
	else 
	{
		this->objectList[id]->prg->setInteger("material.textureEnabled", 0);
	}

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

		this->camera->move(deltaTime);
		move(deltaTime);

		drawObjects();
		
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}