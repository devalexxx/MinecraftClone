//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_CLIENT_MODULE_PLAYER_SESSION_MODULE_H
#define MCC_CLIENT_MODULE_PLAYER_SESSION_MODULE_H

#include "Common/Network/Event.h"

#include <flecs.h>

namespace Mcc
{

	class PlayerSessionModule
	{
		public:
			PlayerSessionModule(flecs::world& world);

		private:
			static void OnConnectionHandler     (flecs::world& world, const OnConnection&    packet);
			static void OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent& event);
	};

}

#endif
