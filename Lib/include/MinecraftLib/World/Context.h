//
// Created by Alex on 18/06/2025.
//

#ifndef MINECRAFT_CONTEXT_H
#define MINECRAFT_CONTEXT_H

#include "MinecraftLib/Network/Packet.h"
#include "MinecraftLib/Network/NetworkID.h"

#include <unordered_map>

namespace Mcc
{

	struct WorldContext
	{
			ServerInfo serverInfo;

			const std::unordered_map<NetworkID, flecs::entity_t>& networkToLocal;
			const std::unordered_map<flecs::entity_t, NetworkID>& localToNetwork;
	};

}

#endif
