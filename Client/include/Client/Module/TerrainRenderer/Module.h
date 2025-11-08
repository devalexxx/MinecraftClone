// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_TERRAIN_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_TERRAIN_RENDERER_MODULE_H

#include "Client/Graphics/Program.h"
#include "Client/Module/TerrainRenderer/Component.h"

#include <flecs.h>

namespace Mcc
{

    class TerrainRendererModule
    {
      public:
        explicit TerrainRendererModule(const flecs::world& world);

        void SetupChunkRenderingMeshSystem(flecs::entity entity, MeshHolder& holder) const;
        void SetupChunkProgramSystem(flecs::iter& it) const;
        void RenderChunkMeshSystem(flecs::iter& it);

      private:
        Program mProgram;
    };

}

#endif
