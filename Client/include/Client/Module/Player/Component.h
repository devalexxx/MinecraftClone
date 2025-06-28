//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_PLAYER_COMPONENT_H
#define MCC_CLIENT_MODULE_PLAYER_COMPONENT_H

#include "Common/Module/Entity/Component.h"

namespace Mcc
{

	struct PlayerEntityTag {};

	struct PlayerEntityPrefab {};

	struct CurrentPlayerInput
	{
			UserInput input;
	};

}

#endif
