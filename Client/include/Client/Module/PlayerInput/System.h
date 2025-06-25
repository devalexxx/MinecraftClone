//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_CLIENT_MODULE_PLAYER_INPUT_SYSTEM_H
#define MCC_CLIENT_MODULE_PLAYER_INPUT_SYSTEM_H

#include "Client/Module/PlayerInput/Component.h"

#include "Common/Module/PlayerEntity/Component.h"

#include <flecs.h>

namespace Mcc
{

	void ApplyAndSendPlayerInput(flecs::iter& it, size_t row, CurrentPlayerInput& current, PlayerInputQueue& queue);

}

#endif
