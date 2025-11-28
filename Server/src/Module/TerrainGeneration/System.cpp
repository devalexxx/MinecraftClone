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
        if (pending.pendingChunk.GetState() == Hx::TaskState::Cancelled)
        {
            // TODO: add a state maybe
            entity.remove<PendingChunk>();
            return;
        }

        if (pending.pendingChunk.GetState() == Hx::TaskState::Done)
        {
            const auto result = pending.pendingChunk.GetResult();
            MCC_ASSERT(result, "Chunk data has already been retrieve");
            holder.chunk = std::make_shared<Chunk>(result->get());
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
                ctx->scheduler.Insert(TerrainReplicationModule::ReplicateChunk, session, world, entity.id()).Enqueue();
            }
        });

        entity.remove<PendingReplication>();
    }

}