//
// Created by Alex Clorennec on 10/07/2025.
//

#include "Client/WorldContext.h"

namespace Mcc
{

    ClientWorldContext* ClientWorldContext::Get(const flecs::world& world)
    {
        return static_cast<ClientWorldContext*>(world.get_ctx());
    }


}