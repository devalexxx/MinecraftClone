// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/Player/System.h"

#include "Client/ClientNetworkManager.h"
#include "Client/Module/Camera/Component.h"
#include "Client/WorldContext.h"

namespace Mcc
{

    void
    ApplyAndSendPlayerInput(const flecs::iter& it, const size_t row, CurrentPlayerInput& current, UserInputQueue& queue)
    {
        const auto* ctx = ClientWorldContext::Get(it.world());

        if (!Helper::IsNull(current.input))
        {
            constexpr float speed = 5.f;
            const float     delta = it.delta_time();

            current.input.meta.id = UserInput::Meta::GetNextID();
            current.input.meta.dt = delta;
            ctx->networkManager.Send<OnPlayerInput>({ current.input }, ENET_PACKET_FLAG_RELIABLE, 0);

            const auto entity = it.entity(row);
            entity.get([&](Transform& transform) {
                Helper::ApplyXAxis(current.input, transform);
                Helper::ApplyMovement(current.input, transform, delta, speed);
            });

            it.world().query_builder<Transform>().with<CameraFollowRelation>(entity).build().each(
                [&](Transform& transform) { Helper::ApplyBothAxis(current.input, transform); }
            );

            queue.data.push_back(current.input);
            current.input.axis = {};
        }
    }

}