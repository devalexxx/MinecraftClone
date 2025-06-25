//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_PLAYER_INPUT_SYSTEM_H
#define MCC_SERVER_MODULE_PLAYER_INPUT_SYSTEM_H

#include "Common/Module/PlayerEntity/Component.h"

#include <flecs.h>

namespace Mcc
{

	void ProcessPlayerInputs(flecs::entity entity, PlayerInputQueue& queue);

}

#endif
