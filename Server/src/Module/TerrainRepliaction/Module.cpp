//
// Created by Alex on 30/06/2025.
//

#include "Server/Module/TerrainReplication/Module.h"
#include "Server/Module/TerrainReplication/Component.h"
#include "Server/Module/TerrainReplication/System.h"

#include "Common/Module/Terrain/Module.h"

#include "Common/Module/Network/Component.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	TerrainReplicationModule::TerrainReplicationModule(const flecs::world& world)
	{
		MCC_ASSERT	 (world.has<TerrainModule>(), "TerrainReplicationModule require TerrainModule, you must import it before.");
		MCC_LOG_DEBUG("Import TerrainReplicationModule...");
		world.module<TerrainReplicationModule>();

		world.component<BlockCreatedTag>();
		world.component<BlockDirtyTag>();
		world.component<BlockDestroyedTag>();

		world.component<ChunkCreatedTag>();
		world.component<ChunkDirtyTag>();
		world.component<ChunkDestroyedTag>();

		world.system<const BlockMeta, const BlockType, const NetworkProps>("BroadcastCreatedBlocksSystem")
			.kind(flecs::PreStore)
			.with<BlockCreatedTag>()
			.run(BroadcastCreatedBlocks);

		world.system<const ChunkPosition, const ChunkHolder, const NetworkProps>("BroadcastCreatedChunksSystem")
			.kind(flecs::OnStore)
			.with<ChunkCreatedTag>()
			.run(BroadcastCreatedChunks);

		world.system<const BlockMeta, const BlockType, const NetworkProps>("BroadcastDirtyBlocksSystem")
			.kind(flecs::OnStore)
			.with<BlockDirtyTag>()
			.run(BroadcastDirtyBlocks);

		world.system<const ChunkPosition, const ChunkHolder, const NetworkProps>("BroadcastDirtyChunksSystem")
			.kind(flecs::OnStore)
			.with<ChunkDirtyTag>()
			.run(BroadcastDirtyChunks);

		world.system<const NetworkProps>("BroadcastDestroyedChunksSystem")
			.kind(flecs::PreStore)
			.with<ChunkDestroyedTag>()
			.run(BroadcastDestroyedChunks);

		world.system<const NetworkProps>("BroadcastDestroyedBlocksSystem")
			.kind(flecs::OnStore)
			.with<BlockDestroyedTag>()
			.run(BroadcastDestroyedBlocks);
	}

}