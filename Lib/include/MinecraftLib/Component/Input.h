//
// Created by Alex on 17/06/2025.
//

#ifndef MINECRAFT_INPUT_H
#define MINECRAFT_INPUT_H

#include <deque>

namespace Mcc
{

	struct PlayerInput;

	using PlayerInputQueue = std::deque<PlayerInput>;

	struct LastInputTracker
	{
			unsigned short id;
	};

}

#endif
