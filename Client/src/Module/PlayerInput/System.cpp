//
// Created by Alex on 23/06/2025.
//

#include "Client/Module/PlayerInput/System.h"
#include "Client/ClientNetworkManager.h"
#include "Client/World/Context.h"

#include "Common/Module/WorldEntity/Component.h"

namespace Mcc
{

	void ApplyAndSendPlayerInput(flecs::iter& it, size_t row, CurrentPlayerInput& current, PlayerInputQueue& queue)
	{
		const auto* ctx = static_cast <ClientWorldContext*>  (it.world().get_ctx());
		const auto& net = dynamic_cast<ClientNetworkManager&>(ctx->networkManager);

		if (current.input.NotNull())
		{
			const float speed = 5.f;
			const float delta = it.delta_time();

			current.input.meta.id = PlayerInputMeta::GetNextID();
			current.input.meta.dt = delta;
			net.Send<OnPlayerInput>({ current.input }, ENET_PACKET_FLAG_RELIABLE, 0);

			it.entity(row).get([&current, speed, delta](Transform& transform) {
				current.input.Apply(transform);
				current.input.Apply(transform, delta, speed);
		  	});

			queue.data.push_back(current.input);
			current.input.axis = {};
		}
	}

}