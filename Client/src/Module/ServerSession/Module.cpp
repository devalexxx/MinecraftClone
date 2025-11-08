// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/ServerSession/Module.h"

#include "Client/Module/ServerSession/Component.h"
#include "Client/WorldContext.h"

#include "Common/Utils/Logging.h"

namespace Mcc
{

    ServerSessionModule::ServerSessionModule(flecs::world& world)
    {
        MCC_LOG_DEBUG("Import ServerSessionModule...");
        world.module<ServerSessionModule>();

        world.component<ServerConnectionState>();

        world.set<ServerConnectionState>(ServerConnectionState::Pending);

        const auto* ctx = ClientWorldContext::Get(world);

        ctx->networkManager.Subscribe<OnWaitingInfo>([&](const auto& packet) { OnWaitingInfoHandler(world, packet); });
        ctx->networkManager.Subscribe<OnConnectionAccepted>([&](const auto& packet) {
            OnConnectionAcceptedHandler(world, packet);
        });
        ctx->networkManager.Subscribe<OnConnectionRefused>([&](const auto& packet) {
            OnConnectionRefusedHandler(world, packet);
        });

        ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event) {
            OnDisconnectEventHandler(world, event);
        });
    }

    void ServerSessionModule::OnWaitingInfoHandler(const flecs::world& world, const OnWaitingInfo&)
    {
        auto* ctx = ClientWorldContext::Get(world);

        ctx->networkManager.Send<OnClientInfo>(
            { static_cast<ClientInfo>(ctx->settings) }, ENET_PACKET_FLAG_RELIABLE, 0
        );
    }

    void ServerSessionModule::OnConnectionAcceptedHandler(const flecs::world& world, const OnConnectionAccepted& packet)
    {
        auto* ctx       = ClientWorldContext::Get(world);
        ctx->playerInfo = packet.playerInfo;
        ctx->serverInfo = packet.serverInfo;

        world.set<ServerConnectionState>(ServerConnectionState::Connected);
    }

    void ServerSessionModule::OnConnectionRefusedHandler(const flecs::world& world, const OnConnectionRefused& packet)
    {
        MCC_LOG_ERROR("Connection refused with: {}", packet.reason);
        world.set<ServerConnectionState>(ServerConnectionState::Error);
    }

    void ServerSessionModule::OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent&)
    {
        world.quit();
    }

}