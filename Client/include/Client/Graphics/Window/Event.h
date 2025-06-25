//
// Created by Alex on 28/08/2024.
//

#ifndef MCC_CLIENT_GRAPHICS_WINDOW_EVENT_H
#define MCC_CLIENT_GRAPHICS_WINDOW_EVENT_H

namespace Mcc
{

	class Window;

	struct KeyEvent
	{
			Window& window;

			int key;
			int scancode;
			int action;
			int mods;
	};

	struct CursorPosEvent
	{
			Window& window;

			double x;
			double y;
	};

}

#endif