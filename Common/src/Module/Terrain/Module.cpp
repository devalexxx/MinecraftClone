//
// Created by Alex on 29/06/2025.
//

#include "Common/Module/Terrain/Module.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Logging.h"

#include <memory>

namespace Mcc
{

	TerrainModule::TerrainModule(flecs::world& world)
	{
		MCC_LOG_DEBUG("Import TerrainModule...");
		world.module<TerrainModule>();

		world.component<BlockTag>();
		world.component<BlockStateTag>();
		world.component<ChunkTag>();

		world.component<BlockStateRelation>();

		world.component<BlockMeta>();

		world.component<ChunkPosition>();
		world.component<ChunkData>();
	}

}