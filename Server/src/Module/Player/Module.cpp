//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/Player/Module.h"
#include "Server/Module/Player/System.h"

#include "Common/Utils/Assert.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Module/Entity/Component.h"
#include "Common/Utils/Logging.h"
#include "Common/World/Context.h"

namespace Mcc
{

	PlayerModule::PlayerModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "PlayerModule require EntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerInputModule...");
		world.module<PlayerModule>();

		world.system<UserInputQueue>().with<UserEntityTag>().each(ProcessPlayerInputs);

		auto* ctx = static_cast<WorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<From<OnPlayerInput>>([&](const auto& event) { OnPlayerInputHandler(world, event); });
	}

	void PlayerModule::OnPlayerInputHandler(flecs::world& world, const From<OnPlayerInput>& from)
	{
		const auto* ctx 	   = static_cast<WorldContext*>(world.get_ctx());
		const auto* playerInfo = static_cast<PlayerInfo*>  (from.peer->data);
		const auto  it 		   = ctx->networkToLocal.find(playerInfo->id);

		if (it == ctx->networkToLocal.cend())
		{
			MCC_LOG_WARN("The network id {} isn't associated to a local entity", playerInfo->id);
			return;
		}

		if (!world.is_alive(it->second))
		{
			MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", playerInfo->id);
			return;
		}

		auto entity = world.entity(it->second);
		entity.get([&from](UserInputQueue& queue) {
			queue.data.push_back(from.packet.input);;
		});
	}

}