//
// Created by Alex on 28/08/2024.
//

#ifndef MCC_CLIENT_GRAPHICS_WINDOW_EVENT_MANAGER_H
#define MCC_CLIENT_GRAPHICS_WINDOW_EVENT_MANAGER_H

#include "Common/Utils/EventManager.h"

#include <functional>
#include <unordered_map>
#include <vector>

struct GLFWwindow;

namespace Mcc
{

	class Window;

	struct WindowEventTag
	{};

	class WindowEventManager : public EventManager<WindowEventTag>
	{
		public:
			static void BoundEvent(const Window& window);

		private:
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void CursorPosCallback(GLFWwindow* window, double x, double y);
	};

}

#endif