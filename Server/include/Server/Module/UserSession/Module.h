//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_USER_SESSION_MODULE_H
#define MCC_SERVER_MODULE_USER_SESSION_MODULE_H

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Network/Component.h"
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
			flecs::query<const Transform, const NetworkProps>                                    mLookupEntityQuery;
			flecs::query<const BlockMeta, const BlockType, const BlockColor, const NetworkProps> mLookupBlockQuery;
			flecs::query<const ChunkPosition, const ChunkHolder, const NetworkProps>             mLookupChunkQuery;

			void OnConnectEventHandler(const flecs::world& world, const ConnectEvent& event) const;

			static void OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent& event);
	};

}

#endif
