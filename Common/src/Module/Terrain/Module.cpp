// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Common/Module/Terrain/Module.h"

#include "Common/Module/Network/Component.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

#include <memory>

namespace Mcc
{

    TerrainModule::TerrainModule(const flecs::world& world)
    {
        MCC_LOG_DEBUG("Import TerrainModule...");
        world.module<TerrainModule>();

        world.component<BlockTag>();
        world.component<BlockStateTag>();
        world.component<ChunkTag>();

        world.component<BlockStateRelation>();

        world.component<BlockMeta>();
        world.component<BlockType>();

        world.component<ChunkPosition>();
        world.component<ChunkHolder>();

        world.prefab<BlockPrefab>()
            .is_a<NetworkObjectPrefab>()
            .add<BlockTag>()
            .set_auto_override<BlockMeta>({})
            .set_auto_override<BlockColor>({})
            .set_auto_override<BlockType>({});

        world.prefab<ChunkPrefab>()
            .is_a<NetworkObjectPrefab>()
            .add<ChunkTag>()
            .add<ChunkPosition>()
            .set_auto_override<ChunkHolder>({});
    }

}