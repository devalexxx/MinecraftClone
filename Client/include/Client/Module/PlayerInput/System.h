//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_MODULE_CLIENT_PLAYER_ENTITY_SYSTEM_H
#define MCC_MODULE_CLIENT_PLAYER_ENTITY_SYSTEM_H

#include <Client/Module/PlayerInput/Component.h>

#include <MinecraftLib/Module/PlayerEntity/Component.h>

#include <flecs.h>

namespace Mcc
{

	void ApplyAndSendPlayerInput(flecs::iter& it, size_t row, CurrentPlayerInput& current, PlayerInputQueue& queue);

}

#endif
