//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_USER_SESSION_MODULE_H
#define MCC_SERVER_MODULE_USER_SESSION_MODULE_H

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Network/Event.h"

#include <flecs.h>

namespace Mcc
{

	class UserSessionModule
	{
		public:
			UserSessionModule(flecs::world& world);

		private:
			flecs::query<const Transform> mLookupEntityQuery;
			flecs::query<const BlockMeta, const BlockType> mLookupBlockQuery;
			flecs::query<const ChunkPosition, const ChunkData> mLookupChunkQuery;

			void OnConnectEventHandler(flecs::world& world, const ConnectEvent& event) const;

			static void OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent& event);
	};

}

#endif
