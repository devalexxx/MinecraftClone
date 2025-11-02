//
// Created by Alex Clorennec on 19/10/2025.
//

#ifndef MCC_SERVER_MODULE_TERRAIN_GENERATION_SYSTEM_H
#define MCC_SERVER_MODULE_TERRAIN_GENERATION_SYSTEM_H

#include <Server/Module/TerrainGeneration/Component.h>

namespace Mcc
{

    void HandleGenerationEndingSystem(flecs::entity entity, PendingChunk& pending, ChunkHolder& holder);
    void DispatchPendingReplication  (flecs::entity entity, GenerationDoneTag);

}

#endif
