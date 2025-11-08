// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_MODULE_TERRAIN_GENERATION_MODULE_H
#define MCC_SERVER_MODULE_TERRAIN_GENERATION_MODULE_H

#include "Server/World/ChunkGenerator.h"

#include <flecs.h>

namespace Mcc
{

    class TerrainGenerationModule
    {
      public:
        TerrainGenerationModule(const flecs::world& world);

        flecs::entity LaunchGenerationTask(const flecs::world& world, const glm::ivec3& position) const;

      private:
        ChunkGenerator mGenerator;
    };

}

#endif
