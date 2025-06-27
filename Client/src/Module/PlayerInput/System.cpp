//
// Created by Alex on 23/06/2025.
//

#include "Client/Module/PlayerInput/System.h"
#include "Client/ClientNetworkManager.h"
#include "Client/World/Context.h"
#include "Client/Module/Camera/Tag.h"

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

			auto entity = it.entity(row);
			entity.get([&](Transform& pTransform) {
				entity.children([&](flecs::entity child) {
					if (child.has<CameraTag>())
					{
						child.get([&](Transform& cTransform) {
							current.input.Apply(pTransform, cTransform);
							current.input.Apply(pTransform, delta, speed);
						});
					}
				});
			});

			queue.data.push_back(current.input);
			current.input.axis = {};
		}
	}

}