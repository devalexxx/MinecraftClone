//
// Created by Alex on 28/08/2024.
//

#ifndef MINECRAFT_WINDOW_H
#define MINECRAFT_WINDOW_H

#include <Client/Graphics/Window/Event.h>
#include <Client/Graphics/Window/EventManager.h>
#include <GLFW/glfw3.h>

namespace Mcc
{

	class Window : public WindowEventManager
	{
		public:
			static void PollEvents();

		public:
			Window(const char* title);
			Window(const char* title, int width, int height);

			~Window();

			[[nodiscard]] std::pair<int, int> GetWindowSize    () const;
			[[nodiscard]] std::pair<int, int> GetWindowPosition() const;
			[[nodiscard]] float               GetAspectRatio   () const;

			void               SetShouldClose() const;
			[[nodiscard]] bool ShouldClose() const;

			bool IsFocused() const;

			void MakeContextCurrent() const;
			void SwapBuffer() const;
			void SetInputMode(int mode, int value) const;

			void SetCursorPosition(int x, int y);

			[[nodiscard]] GLFWwindow* Get() const;

		private:
			GLFWwindow* mWindow;

			static char sWindowCount;
			static bool sIsGlfwInitialize;
			static void InitializeGlfw();

			static bool sIsExtensionLoaded;
			static void LoadExtension();

			static void GlfwErrorHandler(int error_code, const char* description);

			friend class WindowEventManager;
	};

}

#endif
