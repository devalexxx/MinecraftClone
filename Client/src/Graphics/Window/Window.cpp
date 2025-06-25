//
// Created by Alex on 28/08/2024.
//

#include "Client/Graphics/Window/Window.h"

#include "Common/Utils/Logging.h"

#include <fmt/base.h>
#include <glad/glad.h>

namespace Mcc
{

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	Window::Window(const char* title) :
		Window(title, 1024, 720)
	{}

	Window::Window(const char* title, int width, int height)
	{
		if (!sIsGlfwInitialize)
		{
			InitializeGlfw();
		}

#ifdef MACOSX
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef MCC_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif

		if ((mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr)))
		{
			sWindowCount += 1;
			glfwSetWindowUserPointer(mWindow, this);

			if (!sIsExtensionLoaded)
			{
				MakeContextCurrent();
				glfwSwapInterval(0);
				LoadExtension();
			}

			WindowEventManager::BoundEvent(*this);
		}
		else
		{
			MCC_LOG_ERROR("Failed to create window and context");
		}
	}

	Window::~Window()
	{
		glfwDestroyWindow(mWindow);
		sWindowCount--;
		if (sWindowCount == 0 && sIsGlfwInitialize)
		{
			glfwTerminate();
		}
	}

	std::pair<int, int> Window::GetWindowSize() const
	{
		std::pair<int, int> p;
		glfwGetWindowSize(mWindow, &p.first, &p.second);
		return p;
	}

	std::pair<int, int> Window::GetWindowPosition() const
	{
		std::pair<int, int> p;
		glfwGetWindowPos(mWindow, &p.first, &p.second);
		return p;
	}

	float Window::GetAspectRatio() const
	{
		auto [w, h] = GetWindowSize();
		return (float) w / (float) h;
	}

	void Window::SetShouldClose() const
	{
		glfwSetWindowShouldClose(mWindow, true);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(mWindow);
	}

	bool Window::IsFocused() const
	{
		return glfwGetWindowAttrib(mWindow, GLFW_FOCUSED);
	}

	void Window::MakeContextCurrent() const
	{
		glfwMakeContextCurrent(mWindow);
	}

	void Window::SwapBuffer() const
	{
		glfwSwapBuffers(mWindow);
	}

	void Window::SetInputMode(int mode, int value) const
	{
		glfwSetInputMode(mWindow, mode, value);
	}

	void Window::SetCursorPosition(int x, int y)
	{
		glfwSetCursorPos(mWindow, x, y);
	}

	GLFWwindow* Window::Get() const
	{
		return mWindow;
	}

	char Window::sWindowCount      = 0;
	bool Window::sIsGlfwInitialize = false;

	void Window::InitializeGlfw()
	{
		if (glfwInit())
		{
			sIsGlfwInitialize = true;
			glfwSetErrorCallback(GlfwErrorHandler);
		}
		else
		{
			MCC_LOG_ERROR("Failed to initialize GLFW");
		}
	}

	bool Window::sIsExtensionLoaded = false;

	void Window::LoadExtension()
	{
		if (sIsGlfwInitialize)
		{
			if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			{
				sIsExtensionLoaded = true;
			}
			else
			{
				fmt::print("Fail to load OpenGL extensions\n");
			}
		}
		else
		{
			fmt::print("You must initialize GLFW to load OpenGL extension (create a window)\n");
		}
	}

	void Window::GlfwErrorHandler(int error_code, const char* description)
	{
		fmt::print("GLFW Error:\n\tcode: {}\n\tdesc:{}\n", error_code, description);
	}

}