//
// Created by Alex on 23/06/2025.
//

#include "Client/World/Context.h"
#include "Client/Module/ServerSession/Module.h"
#include "Client/Module/ServerSession/Component.h"

#include "Common/Utils/Assert.h"
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

		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnConnection>	  ([&](const auto& packet) { OnConnectionHandler	 (world, packet); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event)  { OnDisconnectEventHandler(world, event);  });
	}


	void ServerSessionModule::OnConnectionHandler(flecs::world& world, const OnConnection& packet)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());
		ctx->playerInfo = packet.playerInfo;
		ctx->serverInfo = packet.serverInfo;

		world.set<InitialWorldState>	({ packet.initialState });
		world.set<ServerConnectionState>(ServerConnectionState::Connected);
	}

	void ServerSessionModule::OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent&)
	{
		world.quit();
	}

}