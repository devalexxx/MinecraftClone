//
// Created by Alex Clorennec on 04/07/2025.
//

#include "Client/Module/Renderer/Module.h"
#include "Client/Module/TerrainRenderer/Component.h"
#include "Client/Module/TerrainRenderer/Module.h"
#include "Client/Module/TerrainRenderer/System.h"
#include "Client/Module/TerrainReplication/Component.h"


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

		world.component<ChunkMesh>();

		world.system<const ChunkHolder, const ChunkPosition>("BuildChunkMesh")
			.kind(flecs::PostUpdate)
			.with<ChunkDirtyTag>()
			.each(BuildChunkMeshSystem);

	    world.system<MeshHolder>("SetupChunkRenderingMesh")
	        .kind(flecs::PostUpdate)
            .each([this](const flecs::entity entity, MeshHolder& holder) { SetupChunkRenderingMeshSystem(entity, holder); } );

		world.system("SetupChunkProgram")
			.kind(flecs::OnStart)
			.run([this](auto&&... args) { SetupChunkProgramSystem(args...); });

		world.system<const ChunkPosition, const ChunkMesh>("RenderChunkMeshSystem")
			.kind(flecs::PreStore)
			.run([this](auto&&... args) { RenderChunkMeshSystem(args...); });
	}


}