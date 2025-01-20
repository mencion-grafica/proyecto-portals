#include "Render.h"
#include "InputManager.h"

Render::Render()
{
	Render::r = this;
	this->objectList = std::vector<Object*>();
	this->cameraList = std::vector<Camera*>();
	this->objectList.reserve(300);
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

	Camera* cam = Render::r->cameraList[0];

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
	Camera* cam = Render::r->cameraList[1];
	cam->fov -= (float) yOffset;
	if (cam->fov < 1.0f) cam->fov = 1.0f;
	if (cam->fov > 45.0f) cam->fov = 45.0f;
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
		std::vector<int> activeLights;
		for (int lightId = 0; lightId < this->lightList.size(); lightId++) if (this->lightList[lightId]->enable) activeLights.push_back(lightId);
		drawGL(obj->id, activeLights);
	}
}

void Render::drawObjects(glm::vec4 position, glm::vec4 direction, Object* portal)
{
	for (auto& obj : this->objectList)
	{
		if(obj == portal)
		{
			continue;
		}
		if(obj->alwaysRender)
		{
			drawGL(obj->id);
			continue;
		}
		glm::vec4 toObject = obj->position - position;
		if (glm::dot(toObject, direction) > 0)
		{
			drawGL(obj->id);
		}
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

void Render::drawGL(int id, const std::vector<int>& lightIds)
{
	glm::mat4 M = this->objectList[id]->modelMatrix;
	glm::mat4 V = activeCamera->computeViewMatrix();
	glm::mat4 P = activeCamera->computeProjectionMatrix();

	glm::mat4 MVP = P * V * M;

	bufferObject_t bo = bufferObjectList[this->objectList[id]->id];
	this->objectList[id]->prg->use();
	glBindVertexArray(bo.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bo.vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.edgeBufferID);

	this->objectList[id]->prg->setMVP(MVP);
	this->objectList[id]->prg->setMatrix("M", M);

	this->objectList[id]->prg->setFloat("material.Ka", this->objectList[id]->material.Ka);
	this->objectList[id]->prg->setFloat("material.Kd", this->objectList[id]->material.Kd);
	this->objectList[id]->prg->setFloat("material.Ks", this->objectList[id]->material.Ks);
	this->objectList[id]->prg->setInteger("material.shiny", this->objectList[id]->material.shiny);

	for (int i = 0; i < lightIds.size(); i++)
	{
		int lightId = lightIds[i];
		this->objectList[id]->prg->setVec4("light.position", this->lightList[lightId]->position);
		this->objectList[id]->prg->setVec4("light.color", this->lightList[lightId]->color);
		this->objectList[id]->prg->setVec4("light.direction", this->lightList[lightId]->direction);
		this->objectList[id]->prg->setFloat("light.Ia", this->lightList[lightId]->Ia);
		this->objectList[id]->prg->setFloat("light.Id", this->lightList[lightId]->Id);
		this->objectList[id]->prg->setFloat("light.Is", this->lightList[lightId]->Is);
		this->objectList[id]->prg->setInteger("light.type", (int)this->lightList[lightId]->type);
		this->objectList[id]->prg->setInteger("light.enable", this->lightList[lightId]->enable);
	}
	
	//for para array de transforms del shader
	this->objectList[id]->prg->setVertexAttribute("vPos", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexPos));
	this->objectList[id]->prg->setVertexAttribute("vColor", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexColor));
	this->objectList[id]->prg->setVertexAttribute("vNormal", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexNormal));
	this->objectList[id]->prg->setVertexAttribute("jointIndex", 4, GL_INT, sizeof(vertex_t), (void*)offsetof(vertex_t, idJoints));
	this->objectList[id]->prg->setVertexAttribute("weightJoints", 4, GL_INT, sizeof(vertex_t), (void*) offsetof(vertex_t, weightJoints));
	this->objectList[id]->prg->setVertexAttribute("vUv", 4, GL_FLOAT, sizeof(vertex_t), (void*) offsetof(vertex_t, vertexUv));

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

void Render::putLight(Light* light)
{
	this->lightList.push_back(light);
}

void Render::putCamera(Camera* camera)
{
	cameraList.push_back(camera);
}


void Render::move(double deltaTime)
{
	for (auto& obj : this->objectList)
	{
		obj->move(deltaTime);
		if (obj->collider != nullptr) obj->collider->computeBounds(obj->modelMatrix, obj->vertexList);
	}
}

void Render::moveLights(float deltaTime)
{
	for (auto& light : this->lightList) light->move(deltaTime);
}


bool Render::checkCollisions(Collider* collider)
{
	for (auto& obj : this->objectList)
	{
		if (obj->collider == nullptr || obj->collider == collider) continue;
		if (obj->collider->checkCollision(collider)) return true;
	}
	return false;
}

bool Render::checkCollisions(glm::vec4 position)
{
	for (auto& obj : this->objectList)
	{
		if (obj->collider == nullptr) continue;
		if (obj->collider->checkCollision(position)) return true;
	}
	return false;
}

void Render::mainLoop() 
{
	double currentTime = 0.0f;
	double lastTime = 0.0f;
	double deltaTime = 0.0f;

    while (!glfwWindowShouldClose(this->window) && !InputManager::keysState[GLFW_KEY_ESCAPE])
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

    	move(deltaTime);

    	//for(int i=cameraList.size()-1;i>=0;i--)
    	activeCamera=cameraList[1];
    	activeCamera->fbt->Bind();
    	activeCamera->move(deltaTime);
    	{
    		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    		
    		drawObjects(activeCamera->pivotPosition, glm::vec4(activeCamera->front.x, activeCamera->front.y, activeCamera->front.z, 1.0f), objectList[5]);
    	}
    	objectList[3]->texture->GLId = cameraList[1]->fbt->GLId;

    	activeCamera=cameraList[2];
    	activeCamera->fbt->Bind();
    	activeCamera->move(deltaTime);
	    {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    		
			drawObjects(activeCamera->pivotPosition, glm::vec4(activeCamera->front.x, activeCamera->front.y, activeCamera->front.z, 1.0f), objectList[3]);
	    }
    	objectList[5]->texture->GLId = cameraList[2]->fbt->GLId;

    	
    	activeCamera=cameraList[0];
    	activeCamera->move(deltaTime);
    	moveLights(deltaTime);
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    		
    	drawObjects();

    	if (this->renderColliders)
    	{
    		// Debugging: Draw colliders
    		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode
    		//glDisable(GL_CULL_FACE);                            // Optional: Disable face culling to see inside
    		glColor3f(1.0f, 0.0f, 0.0f);           // Red color for debugging
    		glLineWidth(5.0f);					          // Line width for debugging
    		for (auto& obj : this->objectList) if (obj->collider != nullptr) obj->collider->draw();
    		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to fill mode
    	}
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}