//
// Created by Alex on 18/06/2025.
//

#ifndef MINECRAFT_CLIENT_CONTEXT_H
#define MINECRAFT_CLIENT_CONTEXT_H

#include "Client/Graphics/Window/Window.h"
#include "MinecraftLib/Network/Packet.h"
#include "MinecraftLib/World/Context.h"

namespace Mcc
{
	struct ClientWorldContext : WorldContext
	{
			PlayerInfo playerInfo;
			Window&    window;
	};
}

#endif
