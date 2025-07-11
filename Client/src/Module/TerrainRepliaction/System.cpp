//
// Created by Alex Clorennec on 07/07/2025.
//

#include "Client/Module/TerrainReplication/System.h"
#include "Client/Module/TerrainReplication/Component.h"

#include "Common/Utils/Logging.h"
#include "Common/WorldContext.h"

namespace Mcc
{

    void DispatchInitialTerrainStateSystem(const flecs::iter& it, size_t, InitialWorldState& iws)
    {
        const auto* ctx = WorldContext<>::Get(it.world());
        ctx->networkManager.Dispatch<OnBlocksCreated>({ std::move(iws.state.blocks) });
        ctx->networkManager.Dispatch<OnChunksCreated>({ std::move(iws.state.chunks) });
    }

	void RemoveChunkDirtySystem(const flecs::entity entity)
	{
		entity.remove<ChunkDirtyTag>();
	}


}