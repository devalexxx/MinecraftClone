// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Server/Module/TerrainGeneration/System.h"

#include "Server/Module/TerrainReplication/Module.h"
#include "Server/WorldContext.h"

namespace Mcc
{

    void HandleGenerationEndingSystem(const flecs::entity entity, PendingChunk& pending, ChunkHolder& holder)
    {
        if (pending.pendingChunk.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            holder.chunk = std::make_shared<Chunk>(pending.pendingChunk.get());
            entity.remove<PendingChunk>();
            entity.remove<GenerationProgressTag>();
            entity.add<GenerationDoneTag>();
        }
    }

    void DispatchPendingReplication(const flecs::entity entity, const GenerationDoneTag)
    {
        entity.get([&](const PendingReplication& pending) {
            const auto world = entity.world();
            const auto ctx   = ServerWorldContext::Get(world);
            for (const auto session: pending.sessions)
            {
                ctx->threadPool.ExecuteTask(TerrainReplicationModule::ReplicateChunk, session, world, entity.id());
            }
        });

        entity.remove<PendingReplication>();
    }

}