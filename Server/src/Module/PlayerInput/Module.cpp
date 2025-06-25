//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/PlayerInput/Module.h"
#include "Server/Module/PlayerInput/System.h"

#include "Common/Utils/Assert.h"
#include "Common/Module/PlayerEntity/Component.h"
#include "Common/Module/PlayerEntity/Tag.h"
#include "Common/Utils/Logging.h"
#include "Common/World/Context.h"

namespace Mcc
{

	PlayerInputModule::PlayerInputModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<PlayerEntityModule>(), "PlayerInputModule require PlayerEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerInputModule...");
		world.module<PlayerInputModule>();

		world.system<PlayerInputQueue>().with<PlayerEntityTag>().each(ProcessPlayerInputs);

		auto* ctx = static_cast<WorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<From<OnPlayerInput>>([&](const auto& event) { OnPlayerInputHandler(world, event); });
	}

	void PlayerInputModule::OnPlayerInputHandler(flecs::world& world, const From<OnPlayerInput>& from)
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
		entity.get([&from](PlayerInputQueue& queue) {
			queue.push_back(from.packet.input);;
		});
	}

}