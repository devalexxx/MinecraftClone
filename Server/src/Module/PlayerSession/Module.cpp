//
// Created by Alex on 23/06/2025.
//

#include <Server/Module/PlayerSession/Module.h>
#include <Server/ServerNetworkManager.h>

#include "MinecraftLib/Module/PlayerEntity/Module.h"
#include <MinecraftLib/Module/PlayerEntity/Prefab.h>
#include <MinecraftLib/Module/WorldEntity/Tag.h>
#include <MinecraftLib/World/Context.h>

namespace Mcc
{

	PlayerSessionModule::PlayerSessionModule(flecs::world& world)
	{
		assert(world.has<PlayerEntityModule>());

		world.module<PlayerSessionModule>();

		auto* ctx = static_cast<WorldContext*>(world.get_ctx());

		mLookupQuery = world.query_builder<const Position, const Rotation>().with<WorldEntityTag>().build();

		ctx->networkManager.Subscribe<ConnectEvent>   ([&](const auto& event) { OnConnectEventHandler   (world, event); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event) { OnDisconnectEventHandler(world, event); });
	}

	void PlayerSessionModule::OnConnectEventHandler(flecs::world& world, const ConnectEvent& event) const
	{
		auto* 		ctx = static_cast <WorldContext*>		   (world.get_ctx());
		const auto& net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		OnConnection packet;
		packet.initialStates = {};

		mLookupQuery
			.run([&](flecs::iter& it) {
				while (it.next())
				{
					auto p = it.field<const Position>(0);
					auto r = it.field<const Rotation>(1);

					for (auto i: it)
					{
						auto entity = it.entity(i);
						packet.initialStates.push_back({ ctx->localToNetwork[entity.id()], p[i], r[i], {} });
					}
				}
			});

		auto id		= GenerateNetworkID();
		auto entity = world.entity().is_a<PlayerEntityPrefab>();
		event.peer->data = new PlayerInfo { id };

		ctx->localToNetwork.emplace(entity.id(), id);
		ctx->networkToLocal.emplace(id, entity.id());

		packet.playerInfo = *static_cast<PlayerInfo*>(event.peer->data);
		packet.serverInfo = ctx->serverInfo;

		net.Send(event.peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		entity.add<WorldEntityCreatedTag>();
	}

	void PlayerSessionModule::OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent& event) const
	{
		const auto* ctx  = static_cast<WorldContext*>(world.get_ctx());
		auto* playerInfo = static_cast<PlayerInfo*>	 (event.peer->data);

		world.entity(ctx->networkToLocal.find(playerInfo->id)->second).add<WorldEntityDestroyedTag>();

		delete playerInfo;
	}

}