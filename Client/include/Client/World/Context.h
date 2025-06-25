//
// Created by Alex on 18/06/2025.
//

#ifndef MCC_CLIENT_WORLD_CONTEXT_H
#define MCC_CLIENT_WORLD_CONTEXT_H

#include "Client/Graphics/Window/Window.h"

#include "Common/Network/Packet.h"
#include "Common/World/Context.h"

namespace Mcc
{
	struct ClientWorldContext : public WorldContext
	{
			PlayerInfo playerInfo;
			Window&    window;
	};
}

#endif
