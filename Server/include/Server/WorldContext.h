// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_WORLD_CONTEXT_H
#define MCC_SERVER_WORLD_CONTEXT_H

#include "Server/ServerNetworkManager.h"
#include "Server/ServerSettings.h"

#include "Common/WorldContext.h"

namespace Mcc
{

    struct ServerWorldContext : WorldContext<ServerNetworkManager>
    {
        static ServerWorldContext* Get(const flecs::world& world);

        ServerSettings settings {};
    };

}

#endif
