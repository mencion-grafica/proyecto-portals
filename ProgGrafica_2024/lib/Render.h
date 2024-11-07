#pragma once

#include "Common.h"
#include "Object.h"
#include "Camera.h"

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

		std::vector<Object*> objectList;

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

		void setupObject(Object* object);

		void move(float deltaTime);

		void drawObjects();

		void initGL(const char* windowName, int sizeX, int sizeY);

		void drawGL(int id);

		void mainLoop();
};
