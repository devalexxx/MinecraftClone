//
// Created by Alex on 18/06/2025.
//

#ifndef MCC_COMMON_WORLD_CONTEXT_H
#define MCC_COMMON_WORLD_CONTEXT_H

#include "Common/Network/Packet.h"
#include "Common/Network/NetworkID.h"
#include "Common/Network/NetworkManager.h"

#include <unordered_map>

namespace Mcc
{

	struct WorldContext
	{
			ServerInfo serverInfo;

			NetworkManager& 							   networkManager;
			std::unordered_map<NetworkID, flecs::entity_t> networkToLocal;
			std::unordered_map<flecs::entity_t, NetworkID> localToNetwork;
	};

}

#endif
