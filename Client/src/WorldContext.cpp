// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/WorldContext.h"

namespace Mcc
{

    ClientWorldContext* ClientWorldContext::Get(const flecs::world& world)
    {
        return static_cast<ClientWorldContext*>(world.get_ctx());
    }


}