// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Server/WorldContext.h"

namespace Mcc
{

    ServerWorldContext* ServerWorldContext::Get(const flecs::world& world)
    {
        return static_cast<ServerWorldContext*>(world.get_ctx());
    }

}