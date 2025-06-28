//
// Created by Alex on 22/06/2025.
//

#include "Server/World/Context.h"
#include "Server/Module/EntityReplication/System.h"
#include "Server/Module/EntityReplication/Component.h"
#include "Server/ServerNetworkManager.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	void BroadcastEntitiesCreated(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto t = it.field<const Transform>(0);
			auto e = it.field<const Extra>	  (1);

			OnEntitiesCreated packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.states.push_back({ id->second, t[i], e[i].data });
					MCC_LOG_INFO("Entity({}) has been created and replicated", id->second);
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for entity({})", entity.id());
				}
				entity.remove<EntityCreatedTag>();
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
			auto t = it.field<const Transform>(0);
			auto e = it.field<const Extra>    (1);

			OnEntitiesUpdated packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.states.push_back({ id->second, t[i], e[i].data });
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for entity({})", entity.id());
				}
				entity.remove<EntityDirtyTag>();
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