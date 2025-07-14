//
// Created by Alex on 01/07/2025.
//

#ifndef MCC_CLIENT_MODULE_TERRAIN_RENDERER_SYSTEM_H
#define MCC_CLIENT_MODULE_TERRAIN_RENDERER_SYSTEM_H

#include "Common/Module/Terrain/Component.h"

#include <flecs.h>

namespace Mcc
{

    void BuildChunkMeshSystem(flecs::entity entity, const ChunkHolder& holder, const ChunkPosition& pos);

}

#endif
