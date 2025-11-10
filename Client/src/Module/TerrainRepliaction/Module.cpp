// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/TerrainReplication/Module.h"

#include "Client/WorldContext.h"

#include "Common/Module/Network/Component.h"
#include "Common/Module/Terrain/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Benchmark.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

    TerrainReplicationModule::TerrainReplicationModule(const flecs::world& world)
    {
        MCC_ASSERT(
            world.has<TerrainModule>(), "TerrainReplicationModule require TerrainModule, you must import it before."
        );
        MCC_LOG_DEBUG("Import TerrainReplication...");
        world.module<TerrainReplicationModule>();

        const auto* ctx = ClientWorldContext::Get(world);

        ctx->networkManager.Subscribe<OnBlock>([&world](const auto& event) { OnBlockHandler(world, event); });
        ctx->networkManager.Subscribe<OnChunk>([&world](const auto& event) { OnChunkHandler(world, event); });

        ctx->networkManager.Subscribe<OnBlockBatch>([&world](const auto& event) { OnBlockBatchHandler(world, event); });
        ctx->networkManager.Subscribe<OnChunkBatch>([&world](const auto& event) { OnChunkBatchHandler(world, event); });
    }

    void TerrainReplicationModule::OnBlockHandler(const flecs::world& world, const Mcc::OnBlock& packet)
    {
        const auto* ctx = ClientWorldContext::Get(world);
        if (const auto lid = ctx->networkMapping.GetLHandle(packet.handle); lid.has_value())
        {
            MCC_LOG_WARN("The network id {} is already associated to a local entity(#{})", packet.handle, *lid);
            return;
        }

        world.entity()
            .is_a<BlockPrefab>()
            .set<NetworkProps>({ packet.handle })
            .set<BlockMeta>(packet.meta)
            .set<BlockColor>({ packet.color })
            .set<BlockType>(packet.type);
    }

    void TerrainReplicationModule::OnChunkHandler(const flecs::world& world, const Mcc::OnChunk& packet)
    {
        auto* ctx = ClientWorldContext::Get(world);
        if (const auto lid = ctx->networkMapping.GetLHandle(packet.handle); lid.has_value())
        {
            MCC_LOG_WARN("The network id {} is already associated to a local entity(#{})", packet.handle, *lid);
            return;
        }

        if (auto from = MCC_BENCH_TIME(RLEDecompression, Helper::FromNetwork)(packet.data, world); from.has_value())
        {
            const auto e = world.entity()
                               .is_a<ChunkPrefab>()
                               .set<NetworkProps>({ packet.handle })
                               .set<ChunkPosition>(packet.position)
                               .set<ChunkHolder>({ std::make_shared<Chunk>(std::move(*from)) });

            ctx->chunkMap.emplace(packet.position.position, e.id());
        }
    }

    void TerrainReplicationModule::OnBlockBatchHandler(const flecs::world& world, const OnBlockBatch& packet)
    {
        for (auto& block: packet) { OnBlockHandler(world, block); }
    }

    void TerrainReplicationModule::OnChunkBatchHandler(const flecs::world& world, const OnChunkBatch& packet)
    {
        for (auto& chunk: packet) { OnChunkHandler(world, chunk); }
    }


}