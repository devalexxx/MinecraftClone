//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_PLAYER_SYSTEM_H
#define MCC_SERVER_MODULE_PLAYER_SYSTEM_H

#include "Common/Module/Entity/Component.h"

#include <flecs.h>

namespace Mcc
{

	void ProcessPlayerInputs(flecs::entity entity, UserInputQueue& queue);

}

#endif
