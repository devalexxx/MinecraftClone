//
// Created by Alex on 22/06/2025.
//

#include <Server/World/Context.h>
#include <Server/Module/EntityReplication/System.h>
#include <Server/ServerNetworkManager.h>

#include <MinecraftLib/Module/WorldEntity/Tag.h>
#include "MinecraftLib/Module/WorldEntity/Component.h"
#include <MinecraftLib/Network/Packet.h>

namespace Mcc
{

	void BroadcastEntitiesCreated(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto p = it.field<const Position>		 (0);
			auto r = it.field<const Rotation>		 (1);
			auto e = it.field<const WorldEntityExtra>(2);

			OnEntitiesCreated packet;
			for (auto i: it)
			{
				flecs::entity entity = it.entity(i);
				packet.states.push_back({ ctx->localToNetwork.find(entity.id())->second, p[i], r[i], e[i].data });
				entity.remove<WorldEntityCreatedTag>();
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastEntitiesUpdated(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto p = it.field<const Position>		 (0);
			auto r = it.field<const Rotation>		 (1);
			auto e = it.field<const WorldEntityExtra>(2);

			OnEntitiesUpdated packet;
			for (auto i: it)
			{
				flecs::entity entity = it.entity(i);
				packet.states.push_back({ ctx->localToNetwork.find(entity.id())->second, p[i], r[i], e[i].data });
				entity.remove<WorldEntityUpdatedTag>();
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastEntitiesDestroyed(flecs::iter& it)
	{
		auto* 		ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto& net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			OnEntitiesDestroyed packet;
			for (auto i: it)
			{
				flecs::entity entity = it.entity(i);
				packet.ids.push_back(ctx->localToNetwork[entity.id()]);

				ctx->networkToLocal.erase(ctx->localToNetwork[entity.id()]);
				ctx->localToNetwork.erase(entity.id());

				entity.destruct();
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

}