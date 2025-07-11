//
// Created by Alex on 23/06/2025.
//

#include "Client/Module/ServerSession/Module.h"
#include "Client/WorldContext.h"
#include "Client/Module/ServerSession/Component.h"

#include "Common/Utils/Logging.h"

namespace Mcc
{

	ServerSessionModule::ServerSessionModule(flecs::world& world)
	{
		MCC_LOG_DEBUG("Import ServerSessionModule...");
		world.module<ServerSessionModule>();

		world.component<InitialWorldState>();
		world.component<ServerConnectionState>();

		world.set<ServerConnectionState>(ServerConnectionState::Pending);

		const auto* ctx = ClientWorldContext::Get(world);

		ctx->networkManager.Subscribe<OnConnection>	  ([&](const auto& packet) { OnConnectionHandler	 (world, packet); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event)  { OnDisconnectEventHandler(world, event);  });
	}


	void ServerSessionModule::OnConnectionHandler(const flecs::world& world, const OnConnection& packet)
	{
		auto* ctx = ClientWorldContext::Get(world);
		ctx->playerInfo = packet.playerInfo;
		ctx->serverInfo = packet.serverInfo;

		world.set<InitialWorldState>	({ packet.initialState });
		world.set<ServerConnectionState>(ServerConnectionState::Connected);
	}

	void ServerSessionModule::OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent&)
	{
		world.quit();
	}

}