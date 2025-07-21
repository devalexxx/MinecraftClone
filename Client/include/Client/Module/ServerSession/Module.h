//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_CLIENT_MODULE_SERVER_SESSION_MODULE_H
#define MCC_CLIENT_MODULE_SERVER_SESSION_MODULE_H

#include "Common/Network/Event.h"
#include "Common/Network/Packet.h"

#include <flecs.h>

namespace Mcc
{

	class ServerSessionModule
	{
		public:
			ServerSessionModule(flecs::world& world);

		private:
			static void OnWaitingInfoHandler       (const flecs::world& world, const OnWaitingInfo&);
			static void OnConnectionAcceptedHandler(const flecs::world& world, const OnConnectionAccepted& packet);
			static void OnConnectionRefusedHandler (const flecs::world& world, const OnConnectionRefused&  packet);

			static void OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent& event);
	};

}

#endif
