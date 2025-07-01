//
// Created by Alex on 01/07/2025.
//

#ifndef MCC_CLIENT_MODULE_TERRAIN_REPLICATION_MODULE_H
#define MCC_CLIENT_MODULE_TERRAIN_REPLICATION_MODULE_H

#include "Common/Network/Packet.h"
#include <flecs.h>

namespace Mcc
{

	class TerrainReplicationModule
	{
		public:
			TerrainReplicationModule(flecs::world& world);

		private:
			static void OnBlocksCreatedHandler(flecs::world& world, const OnBlocksCreated& event);
			static void OnChunksCreatedHandler(flecs::world& world, const OnChunksCreated& event);

			static void OnBlocksUpdatedHandler(flecs::world& world, const OnBlocksUpdated& event);
			static void OnChunksUpdatedHandler(flecs::world& world, const OnChunksUpdated& event);

			static void OnChunksDestroyedHandler(flecs::world& world, const OnChunksDestroyed& event);
			static void OnBlocksDestroyedHandler(flecs::world& world, const OnBlocksDestroyed& event);
	};

}

#endif