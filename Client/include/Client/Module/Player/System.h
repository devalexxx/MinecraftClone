//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_PLAYER_SYSTEM_H
#define MCC_CLIENT_MODULE_PLAYER_SYSTEM_H

#include "Client/Module/Player/Component.h"

#include "Common/Module/Entity/Component.h"

#include <flecs.h>

namespace Mcc
{

	void ApplyAndSendPlayerInput(const flecs::iter& it, size_t row, CurrentPlayerInput& current, UserInputQueue& queue);

}

#endif
