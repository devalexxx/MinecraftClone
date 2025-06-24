//
// Created by Alex on 23/06/2025.
//

#include <Server/Module/PlayerInput/Module.h>
#include <Server/Module/PlayerInput/System.h>

#include <MinecraftLib/Module/PlayerEntity/Component.h>
#include <MinecraftLib/Module/PlayerEntity/Tag.h>
#include <MinecraftLib/World/Context.h>

namespace Mcc
{

	PlayerInputModule::PlayerInputModule(flecs::world& world)
	{
		assert(world.has<PlayerEntityModule>());

		world.module<PlayerInputModule>();

		world.system<PlayerInputQueue>().with<PlayerEntityTag>().each(ProcessPlayerInputs);

		auto* ctx  = static_cast<WorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<From<OnPlayerInput>>([&](const auto& event) { OnPlayerInputHandler    (world, event); });
	}

	void PlayerInputModule::OnPlayerInputHandler(flecs::world& world, const From<OnPlayerInput>& from)
	{
		const auto* ctx 	   = static_cast<WorldContext*>(world.get_ctx());
		const auto* playerInfo = static_cast<PlayerInfo*>  (from.peer->data);

		if (world.exists(ctx->networkToLocal.find(playerInfo->id)->second))
		{
			auto entity = world.entity(ctx->networkToLocal.find(playerInfo->id)->second);
			entity.get_ref<PlayerInputQueue>()->push_back(from.packet.input);
		}
	}

}