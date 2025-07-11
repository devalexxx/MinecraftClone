//
// Created by Alex on 18/06/2025.
//

#ifndef MCC_CLIENT_WORLD_CONTEXT_H
#define MCC_CLIENT_WORLD_CONTEXT_H

#include "Client/Graphics/Window/Window.h"
#include "Client/ClientNetworkManager.h"

#include "Common/WorldContext.h"
#include "Common/Network/Packet.h"

namespace Mcc
{

	struct ClientWorldContext : WorldContext<ClientNetworkManager>
	{
	        static ClientWorldContext* Get(const flecs::world& world);

			PlayerInfo playerInfo{};
			Window&    window;
	};

}

#endif
