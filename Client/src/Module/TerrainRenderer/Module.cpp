//
// Created by Alex Clorennec on 04/07/2025.
//

#include "Client/Module/Renderer/Module.h"
#include "Client/Module/TerrainRenderer/Component.h"
#include "Client/Module/TerrainRenderer/Module.h"

#include "Client/Module/TerrainRenderer/System.h"
#include "Client/Module/TerrainReplication/Component.h"
#include "Client/WorldContext.h"


#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	TerrainRendererModule::TerrainRendererModule(const flecs::world& world)
	{
		MCC_ASSERT(world.has<RendererModule>(), "TerrainRendererModule require RendererModule, you must import it before.");
		MCC_LOG_DEBUG("Import TerrainRendererModule...");
		world.module<TerrainRendererModule>();

        world.component<ShouldBuildMeshTag>();
        world.component<CouldRenderChunkTag>();
        world.component<ShouldRenderChunkTag>();

		world.component<ChunkMesh>();
		world.component<MeshHolder>();

	    const auto* ctx = ClientWorldContext::Get(world);

	    world.observer<const Transform>("OnPlayerMove")
	        .event(flecs::OnSet)
	        .term_at(0).src(world.entity(*ctx->networkMapping.GetLHandle(ctx->playerInfo.handle)))
	        .each(OnPlayerMoveObserver);

        world.observer<const ChunkPosition>("OnChunkCreated")
	        .event(flecs::OnSet)
            .each(OnChunkCreatedObserver);

	    world.observer<const ChunkHolder>("OnChunkChanged")
	        .event(flecs::OnSet)
	        .with<CouldRenderChunkTag>()
	        .each(OnChunkChangedObserver);

		world.system<const ChunkHolder, const ChunkPosition>("BuildChunkMesh")
			.kind(flecs::PostUpdate)
			.with<ShouldBuildMeshTag>()
			.each(BuildChunkMeshSystem);

	    world.system<MeshHolder>("SetupChunkRenderingMesh")
	        .kind(flecs::PostUpdate)
            .each([this](const flecs::entity entity, MeshHolder& holder) { SetupChunkRenderingMeshSystem(entity, holder); } );

		world.system("SetupChunkProgram")
			.kind(flecs::OnStart)
			.run([this](auto&&... args) { SetupChunkProgramSystem(args...); });

		world.system<const ChunkPosition, const ChunkMesh>("RenderChunkMeshSystem")
			.kind(flecs::PreStore)
	        .with<ShouldRenderChunkTag>()
			.run([this](auto&&... args) { RenderChunkMeshSystem(args...); });
	}


}