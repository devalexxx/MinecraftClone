//
// Created by Alex on 23/06/2025.
//

#include "Client/World/Context.h"
#include "MinecraftLib/Module/WorldEntity/Prefab.h"
#include <Client/Module/PlayerSession/Component.h>
#include <Client/Module/PlayerSession/Module.h>

namespace Mcc
{

	PlayerSessionModule::PlayerSessionModule(flecs::world& world)
	{
		world.module<PlayerSessionModule>();

		world.component<ServerConnectionState>();

		world.set<ServerConnectionState>(ServerConnectionState::Pending);

		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnConnection>	  ([&](const auto& packet) { OnConnectionHandler	 (world, packet); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event)  { OnDisconnectEventHandler(world, event);  });
	}


	void PlayerSessionModule::OnConnectionHandler(flecs::world& world, const OnConnection& packet)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());
		ctx->playerInfo = packet.playerInfo;
		ctx->serverInfo = packet.serverInfo;

		for (auto& state: packet.initialStates)
		{
			auto entity = world.entity()
				  .is_a<WorldEntityPrefab>()
				  .set(state.position)
				  .set(state.rotation)
				  .set(Forward::FromRotation(state.rotation))
				  .set(Right  ::FromRotation(state.rotation));

			ctx->localToNetwork.emplace(entity.id(), state.id);
			ctx->networkToLocal.emplace(state.id, entity.id());
		}

		world.set<ServerConnectionState>(ServerConnectionState::Connected);
	}

	void PlayerSessionModule::OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent&)
	{
		world.quit();
	}

}