// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Server/Module/Player/System.h"

#include "Server/Module/EntityReplication/Component.h"
#include "Server/Module/Player/Component.h"
#include "Server/Module/TerrainGeneration/Module.h"
#include "Server/Module/UserSession/Module.h"
#include "Server/WorldContext.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Utils/ChunkHelper.h"

#include <ranges>

namespace Mcc
{

    void ProcessPlayerInputs(const flecs::entity entity, UserInputQueue& queue)
    {
        const auto* ctx = ServerWorldContext::Get(entity.world());

        if (queue.data.empty())
        {
            return;
        }

        entity.get([&](Transform& transform, Extra& extra) {
            const auto position = transform.position;
            for (; !queue.data.empty(); queue.data.pop_front())
            {
                auto& input = queue.data.front();
                Helper::ApplyXAxis(input, transform);
                Helper::ApplyMovement(input, transform, ctx->settings.userSpeed, input.meta.dt);

                if (queue.data.size() == 1)
                {
                    extra.data.insert_or_assign("last-input-processed", std::to_string(input.meta.id));
                }
            }

            entity.world()
                .event<OnPlayerMoveEvent>()
                .ctx({ .prev = position, .curr = transform.position })
                .entity(entity)
                .emit();
        });

        entity.add<EntityDirtyTag>();
    }

    void HandlePlayerCreation(const flecs::entity entity)
    {
        entity.world().event<OnPlayerCreatedEvent>().id<Transform>().entity(entity).emit();
    }

}