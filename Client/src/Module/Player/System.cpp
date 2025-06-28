//
// Created by Alex on 28/06/2025.
//

#include "Client/Module/Player/System.h"
#include "Client/ClientNetworkManager.h"
#include "Client/Module/Camera/Component.h"
#include "Client/World/Context.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	void ApplyAndSendPlayerInput(flecs::iter& it, size_t row, CurrentPlayerInput& current, UserInputQueue& queue)
	{
		const auto* ctx = static_cast <ClientWorldContext*>  (it.world().get_ctx());
		const auto& net = dynamic_cast<ClientNetworkManager&>(ctx->networkManager);

		if (!Helper::IsNull(current.input))
		{
			const float speed = 5.f;
			const float delta = it.delta_time();

			current.input.meta.id = UserInput::Meta::GetNextID();
			current.input.meta.dt = delta;
			net.Send<OnPlayerInput>({ current.input }, ENET_PACKET_FLAG_RELIABLE, 0);

			auto entity = it.entity(row);
			entity.get([&](Transform& transform) {
				Helper::ApplyXAxis	 (current.input, transform);
				Helper::ApplyMovement(current.input, transform, delta, speed);
			});

			it.world().query_builder<Transform>().with<CameraFollowRelation>(entity).build().each([&](Transform& transform) {
				Helper::ApplyBothAxis(current.input, transform);
			});

			queue.data.push_back(current.input);
			current.input.axis = {};
		}
	}

}