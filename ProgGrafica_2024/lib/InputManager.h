#pragma once

#include "Common.h"

/**
 * @brief The InputManager class handles input events such as keyboard and mouse interactions.
 */
class InputManager
{
	public:
		static inline std::map<int, bool> keysState;

		typedef struct
		{
			double posX, posY;
			std::map<int, bool> buttons;
		} mouseState_t;

		static inline mouseState_t mouseState = { 0, 0 };

		static void mouseButtonManager(GLFWwindow* window, int button, int action, int mods);

		static void mousePosManager(GLFWwindow* window, double x, double y);

		static void keyboardManager(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void initInputManager(GLFWwindow* window);
};