#pragma once

#include "Common.h"
#include "Object.h"

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

		std::vector<Object*> objectList;

		Render();

		void putObject(Object* object);

		void initGL(const char* windowName, int sizeX, int sizeY);

		void drawGL();

		void mainLoop();
};