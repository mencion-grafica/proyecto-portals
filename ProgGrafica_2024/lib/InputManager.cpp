#include "InputManager.h"

void InputManager::mouseButtonManager(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			mouseState.buttons[button] = true;
		} break;
		
		case GLFW_RELEASE:
		{
			mouseState.buttons[button] = false;
		} break;
	}
}

void InputManager::keyboardManager(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			keysState[key] = true;
		} break;

		case GLFW_RELEASE:
		{
			keysState[key] = false;
		} break;
	}
}

void InputManager::initInputManager(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyboardManager);
	glfwSetMouseButtonCallback(window, mouseButtonManager);
}