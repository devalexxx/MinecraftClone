// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_WORLD_CONTEXT_H
#define MCC_CLIENT_WORLD_CONTEXT_H

#include "Client/ClientNetworkManager.h"
#include "Client/ClientSettings.h"
#include "Client/Graphics/Window/Window.h"

#include "Common/Network/Packet.h"
#include "Common/WorldContext.h"

namespace Mcc
{

    struct ClientWorldContext : WorldContext<ClientNetworkManager>
    {
        static ClientWorldContext* Get(const flecs::world& world);

        ServerInfo     serverInfo;
        PlayerInfo     playerInfo;
        ClientSettings settings;
        Window&        window;
    };

}

#endif
