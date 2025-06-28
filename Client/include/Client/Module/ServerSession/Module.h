//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_CLIENT_MODULE_SERVER_SESSION_MODULE_H
#define MCC_CLIENT_MODULE_SERVER_SESSION_MODULE_H

#include "Common/Network/Event.h"

#include <flecs.h>

namespace Mcc
{

	class ServerSessionModule
	{
		public:
			ServerSessionModule(flecs::world& world);

		private:
			static void OnConnectionHandler     (flecs::world& world, const OnConnection&    packet);
			static void OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent& event);
	};

}

#endif
