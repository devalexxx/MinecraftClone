// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_TERRAIN_REPLICATION_MODULE_H
#define MCC_CLIENT_MODULE_TERRAIN_REPLICATION_MODULE_H

#include "Common/Module/Base/Module.h"
#include "Common/Module/Terrain/Module.h"
#include "Common/Network/Packet.h"

#include <flecs.h>

namespace Mcc
{

    class TerrainReplicationModule final : public BaseModule<TerrainReplicationModule, TerrainModule>
    {
      public:
        explicit TerrainReplicationModule(flecs::world& world);

        void RegisterComponent(flecs::world& world) override;
        void RegisterSystem(flecs::world& world) override;
        void RegisterHandler(flecs::world& world) override;

      private:
        static void OnBlockHandler(const flecs::world& world, const OnBlock& packet);
        static void OnChunkHandler(const flecs::world& world, const OnChunk& packet);

        static void OnBlockBatchHandler(const flecs::world& world, const OnBlockBatch& packet);
        static void OnChunkBatchHandler(const flecs::world& world, const OnChunkBatch& packet);
    };

}

#endif