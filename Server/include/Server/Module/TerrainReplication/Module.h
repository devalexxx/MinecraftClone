// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_MODULE_TERRAIN_MODULE_H
#define MCC_SERVER_MODULE_TERRAIN_MODULE_H

#include "Server/Module/UserSession/Module.h"

#include <flecs.h>

namespace Mcc
{

    struct TerrainReplicationModule
    {
        TerrainReplicationModule(const flecs::world& world);

        static void ReplicateChunk(UserSession* session, const flecs::world& world, flecs::entity_t chunk);
    };

}

#endif
