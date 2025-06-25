//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_PLAYER_INPUT_MODULE_H
#define MCC_SERVER_MODULE_PLAYER_INPUT_MODULE_H

#include "Common/Module/PlayerEntity/Module.h"
#include "Common/Network/Event.h"
#include "Common/Network/Packet.h"

namespace Mcc
{

	class PlayerInputModule
	{
		public:
			PlayerInputModule(flecs::world& world);

		private:
			static void OnPlayerInputHandler(flecs::world& world, const From<OnPlayerInput>& from);
	};

}

#endif
