//
// Created by Alex on 22/06/2025.
//

#include <Server/World/Context.h>
#include <Server/Module/EntityReplication/System.h>
#include <Server/ServerNetworkManager.h>

#include "MinecraftLib/Module/WorldEntity/Component.h"
#include "MinecraftLib/Utils/Logging.h"
#include <MinecraftLib/Module/WorldEntity/Tag.h>
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
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.states.push_back({ id->second, p[i], r[i], e[i].data });
					MCC_LOG_INFO("Entity({}) has been created and replicated", id->second);
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for entity({})", entity.id());
				}
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
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.states.push_back({ id->second, p[i], r[i], e[i].data });
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for entity({})", entity.id());
				}
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
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.ids.push_back(id->second);

					MCC_LOG_INFO("Entity({}) has been destroyed and replicated", id->second);

					ctx->networkToLocal.erase(id->second);
					ctx->localToNetwork.erase(id->first);
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for entity({})", entity.id());
				}

				entity.destruct();
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

}