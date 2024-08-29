//
// Created by Alex on 28/08/2024.
//

#include <Client/Graphics/Window/EventManager.h>
#include <Client/Graphics/Window/Window.h>

namespace Mcc
{

	void WindowEventManager::BoundEvent(const Mcc::Window& window)
	{
		glfwSetKeyCallback(window.Get(), KeyCallback);
		glfwSetCursorPosCallback(window.Get(), CursorPosCallback);
	}

	void WindowEventManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (w)
		{
			w->Dispatch<KeyEvent>({ *w, key, scancode, action, mods });
		}
		else
		{
			fmt::print("Trying to dispatch an event from a non exiting window (Should not happen)\n");
		}
	}

	void WindowEventManager::CursorPosCallback(GLFWwindow* window, double x, double y)
	{
		auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (w)
		{
			w->Dispatch<CursorPosEvent>({ *w, x, y });
		}
		else
		{
			fmt::print("Trying to dispatch an event from a non exiting window (Should not happen)\n");
		}
	}

}