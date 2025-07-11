//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/Player/Module.h"
#include "Server/Module/Player/System.h"

#include "Common/WorldContext.h"
#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	PlayerModule::PlayerModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "PlayerModule require EntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerInputModule...");
		world.module<PlayerModule>();

		world.system<UserInputQueue>().with<UserEntityTag>().each(ProcessPlayerInputs);

        const auto* ctx = WorldContext<>::Get(world);

		ctx->networkManager.Subscribe<From<OnPlayerInput>>([&](const auto& event) { OnPlayerInputHandler(world, event); });
	}

	void PlayerModule::OnPlayerInputHandler(const flecs::world& world, const From<OnPlayerInput>& from)
	{
		const auto* ctx    = WorldContext<>::Get(world);
		const auto* info   = static_cast<PlayerInfo*>(from.peer->data);
		const auto  handle = ctx->networkMapping.GetLHandle(info->handle);

		if (!handle.has_value())
		{
			MCC_LOG_WARN("The network id {} isn't associated to a local entity", info->handle);
			return;
		}

		if (!world.is_alive(*handle))
        {
            MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", info->handle);
            return;
        }

		world.entity(*handle).get([&from](UserInputQueue& queue) {
			queue.data.push_back(from.packet.input);;
		});
	}

}