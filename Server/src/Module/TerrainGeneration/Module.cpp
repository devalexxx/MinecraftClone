//
// Created by Alex Clorennec on 19/10/2025.
//

#include "Server/Module/TerrainGeneration/Module.h"
#include "Server/Module/TerrainGeneration/Component.h"
#include "Server/Module/TerrainGeneration/System.h"
#include "Server/WorldContext.h"

#include "Common/Utils/Logging.h"

namespace Mcc
{

    TerrainGenerationModule::TerrainGenerationModule(const flecs::world& world) :
        mGenerator(123456789u)
    {
        MCC_LOG_DEBUG("Import TerrainGenerationModule...");
        world.module<TerrainGenerationModule>();

        world.component<GenerationPlannedTag>();
        world.component<GenerationProgressTag>();
        world.component<GenerationDoneTag>();

        world.component<PendingChunk>();
        world.component<PendingReplication>();

        world.system<PendingChunk, ChunkHolder>("HandleGenerationEndingSystem")
            .with<GenerationProgressTag>()
            .each(HandleGenerationEndingSystem);

        world.observer<const GenerationDoneTag>("DispatchPendingReplication")
            .event(flecs::OnAdd)
            .with<PendingReplication>()
            .each(DispatchPendingReplication);

        world.entity("mcc:block:air")
            .is_a<BlockPrefab>()
            .set<BlockType>(BlockType::Gas)
            .set<BlockMeta>({ "mcc:block:air" });

        world.entity("mcc:block:stone")
            .is_a<BlockPrefab>()
            .set<BlockType>(BlockType::Solid)
            .set<BlockColor>({ { .5f, .5f, .5f } })
            .set<BlockMeta>({ "mcc:block:stone" });

        world.entity("mcc:block:dirt")
            .is_a<BlockPrefab>()
            .set<BlockType>(BlockType::Solid)
            .set<BlockColor>({ { .0f, .7f, .3f } })
            .set<BlockMeta>({ "mcc:block:dirt" });

        mGenerator.Setup(world);
    }

    flecs::entity TerrainGenerationModule::LaunchGenerationTask(const flecs::world& world, const glm::ivec3& position) const
    {
        const auto ctx    = ServerWorldContext::Get(world);
        const auto entity = world.entity()
            .is_a<ChunkPrefab>()
            .set<ChunkPosition>({ position })
            .add<GenerationProgressTag>();

        auto future = ctx->threadPool.ExecuteTask([=, this] {
            return mGenerator.Generate(position);
        });

        entity.set<PendingChunk>({ std::move(future) });
        ctx->chunkMap[position] = entity.id();

        return entity;
    }


}