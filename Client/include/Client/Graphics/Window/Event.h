//
// Created by Alex on 28/08/2024.
//

#pragma once

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