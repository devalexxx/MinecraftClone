//
// Created by Alex Clorennec on 10/07/2025.
//

#include "Server/WorldContext.h"

namespace Mcc
{

    ServerWorldContext* ServerWorldContext::Get(const flecs::world& world)
    {
        return static_cast<ServerWorldContext*>(world.get_ctx());
    }

}