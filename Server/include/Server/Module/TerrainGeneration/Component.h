// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_MODULE_TERRAIN_GENERATION_COMPONENT_H
#define MCC_SERVER_MODULE_TERRAIN_GENERATION_COMPONENT_H

#include "Server/Module/UserSession/Module.h"

#include <future>
#include <vector>

namespace Mcc
{

    struct GenerationPlannedTag
    {};
    struct GenerationProgressTag
    {};
    struct GenerationDoneTag
    {};

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
