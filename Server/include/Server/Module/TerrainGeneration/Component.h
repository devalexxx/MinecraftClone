//
// Created by Alex Clorennec on 19/10/2025.
//

#ifndef MCC_SERVER_MODULE_TERRAIN_GENERATION_COMPONENT_H
#define MCC_SERVER_MODULE_TERRAIN_GENERATION_COMPONENT_H

#include "Server/Module/UserSession/Module.h"

#include <vector>
#include <future>

namespace Mcc
{

    struct GenerationPlannedTag  {};
    struct GenerationProgressTag {};
    struct GenerationDoneTag     {};

    struct PendingChunk
    {
        std::future<Chunk> pendingChunk;
    };

    struct PendingReplication
    {
        std::vector<UserSession*> sessions;
    };

}

#endif
