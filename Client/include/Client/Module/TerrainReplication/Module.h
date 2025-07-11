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
			TerrainReplicationModule(const flecs::world& world);

		private:
			static void OnBlocksCreatedHandler(const flecs::world& world, const OnBlocksCreated& event);
			static void OnChunksCreatedHandler(const flecs::world& world, const OnChunksCreated& event);

			static void OnBlocksUpdatedHandler(const flecs::world& world, const OnBlocksUpdated& event);
			static void OnChunksUpdatedHandler(const flecs::world& world, const OnChunksUpdated& event);

			static void OnChunksDestroyedHandler(const flecs::world& world, const OnChunksDestroyed& event);
			static void OnBlocksDestroyedHandler(const flecs::world& world, const OnBlocksDestroyed& event);
	};

}

#endif