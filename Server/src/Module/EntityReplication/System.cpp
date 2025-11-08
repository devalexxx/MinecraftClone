// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Server/Module/EntityReplication/System.h"

#include "Server/Module/EntityReplication/Component.h"
#include "Server/ServerNetworkManager.h"
#include "Server/WorldContext.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Network/Component.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

    void BroadcastEntitiesCreated(flecs::iter& it)
    {
        const auto* ctx = ServerWorldContext::Get(it.world());

        while (it.next())
        {
            auto t = it.field<const Transform>(0);
            auto e = it.field<const Extra>(1);
            auto n = it.field<const NetworkProps>(2);

            OnEntitiesCreated packet;
            for (const auto i: it)
            {
                const auto entity = it.entity(i);
                const auto handle = n[i].handle;

                if (!IsValid(handle))
                {
                    MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
                    continue;
                }

                packet.states.push_back({ handle, t[i], e[i].data });
                entity.remove<EntityCreatedTag>();
                MCC_LOG_INFO("Entity({}) has been created and replicated", handle);
            }
            ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
        }
    }

    void BroadcastEntitiesUpdated(flecs::iter& it)
    {
        const auto* ctx = ServerWorldContext::Get(it.world());

        while (it.next())
        {
            auto t = it.field<const Transform>(0);
            auto e = it.field<const Extra>(1);
            auto n = it.field<const NetworkProps>(2);

            OnEntitiesUpdated packet;
            for (const auto i: it)
            {
                const auto entity = it.entity(i);
                const auto handle = n[i].handle;

                if (!IsValid(handle))
                {
                    MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
                    entity.destruct();
                    continue;
                }

                packet.states.push_back({ handle, t[i], e[i].data });
                entity.remove<EntityDirtyTag>();
            }
            ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
        }
    }

    void BroadcastEntitiesDestroyed(flecs::iter& it)
    {
        const auto* ctx = ServerWorldContext::Get(it.world());

        while (it.next())
        {
            auto n = it.field<const NetworkProps>(0);

            OnEntitiesDestroyed packet;
            for (const auto i: it)
            {
                const auto entity = it.entity(i);
                const auto handle = n[i].handle;

                if (!IsValid(handle))
                {
                    MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
                    entity.destruct();
                    continue;
                }

                packet.handles.push_back(handle);
                entity.destruct();
                MCC_LOG_INFO("Entity({}) has been destroyed and replicated", handle);
            }
            ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
        }
    }

}