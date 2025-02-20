#pragma once

#include "Common.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"

/**
 * @class Render
 * @brief The Render class represents a rendering engine for graphics programming.
 *
 * This class provides functionality to initialize OpenGL, manage objects and cameras,
 * and perform rendering operations.
 */
class Render
{
	public:
		static inline Render* r;

		GLFWwindow* window;
		int width;
		int height;

		bool renderColliders = false;

		std::vector<Object*> objectList;
		std::vector<Light*> lightList;

		typedef struct {
			unsigned int bufferID;
			unsigned int vertexBufferID;
			unsigned int edgeBufferID;
		} bufferObject_t;

		std::map<int, bufferObject_t> bufferObjectList;

		Camera* camera;

		Render();

		void putObject(Object* object);

		void putCamera(Camera* camera);

		void putLight(Light* light);
	
		void setupObject(Object* object);

		void move(float deltaTime);

		void moveLights(float deltaTime);

		bool checkCollisions(Collider* collider);
		bool checkCollisions(glm::vec4 position);
	
		void drawObjects();

		void initGL(const char* windowName, int sizeX, int sizeY);

		void drawGL(int id, const std::vector<int>& lightIds);

		void mainLoop();
};
