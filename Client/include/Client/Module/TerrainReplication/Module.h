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
            explicit TerrainReplicationModule(const flecs::world& world);

		private:
            static void OnBlockHandler(const flecs::world& world, const OnBlock& packet);
            static void OnChunkHandler(const flecs::world& world, const OnChunk& packet);

            static void OnBlockBatchHandler(const flecs::world& world, const OnBlockBatch& packet);
	        static void OnChunkBatchHandler(const flecs::world& world, const OnChunkBatch& packet);
	};

}

#endif