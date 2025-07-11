//
// Created by Alex on 01/07/2025.
//

#ifndef MCC_CLIENT_MODULE_TERRAIN_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_TERRAIN_RENDERER_MODULE_H

#include "Client/Graphics/Program.h"

#include "Common/Module/Terrain/Component.h"

#include <flecs.h>

namespace Mcc
{

	class  TerrainRendererModule
	{
		public:
			TerrainRendererModule(const flecs::world& world);

			void BuildChunkMeshSystem   (flecs::entity entity, ChunkHolder& holder) const;
			void SetupChunkProgramSystem(flecs::iter& it) const;
			void RenderChunkMeshSystem  (flecs::iter& it);

		private:
			Program mProgram;
	};

}

#endif
