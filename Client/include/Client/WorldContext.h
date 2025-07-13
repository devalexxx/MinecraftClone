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

    struct IVec3Hasher
    {
        size_t operator()(const glm::ivec3& v) const
        {
            return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) << 1 ^ std::hash<int>()(v.z) << 2;
        }
    };

	struct ClientWorldContext : WorldContext<ClientNetworkManager>
	{
	        static ClientWorldContext* Get(const flecs::world& world);

			PlayerInfo playerInfo{};
			Window&    window;

	        std::unordered_map<glm::ivec3, flecs::entity_t, IVec3Hasher> chunkMap;
	};

}

#endif
