//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_COMMON_MODULE_PLAYER_ENTITY_COMPONENT_H
#define MCC_COMMON_MODULE_PLAYER_ENTITY_COMPONENT_H

#include "Common/PlayerInput.h"

#include <deque>

namespace Mcc
{

	struct PlayerInputQueue
	{
			std::deque<PlayerInput> data;
	};

}

#endif
