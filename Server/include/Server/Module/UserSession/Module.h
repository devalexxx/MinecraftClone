//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_USER_SESSION_MODULE_H
#define MCC_SERVER_MODULE_USER_SESSION_MODULE_H

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Network/Component.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Network/Event.h"
#include "Common/Network/Packet.h"
#include "Common/Network/Packet/Session.h"

#include <flecs.h>

namespace Mcc
{

    struct UserSession
    {
        static UserSession* Get(const ENetPeer* peer);

        PlayerInfo pInfo;
        ClientInfo cInfo;

        ENetPeer*  peer;
        std::unordered_set<flecs::entity_t> replicatedChunks;
        std::unordered_set<flecs::entity_t> replicatedBlocks;
    };

	class UserSessionModule
	{
		public:
            using EntityQuery = flecs::query<const Transform, const NetworkProps>;
	        using BlockQuery  = flecs::query<const BlockMeta, const BlockType, const BlockColor, const NetworkProps>;
	        using ChunkQuery  = flecs::query<const ChunkPosition, const ChunkHolder, const NetworkProps>;

			UserSessionModule(flecs::world& world);

		private:
			static void OnConnectEventHandler   (const flecs::world& world, const ConnectEvent&    event);
	        static void OnClientInfoHandler     (const flecs::world& world, const From<OnClientInfo>& from);
			static void OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent& event);
	};

}

#endif
