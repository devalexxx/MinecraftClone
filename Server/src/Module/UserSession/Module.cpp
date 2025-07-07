//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/UserSession/Module.h"
#include "Server/Module/EntityReplication/Component.h"
#include "Server/ServerNetworkManager.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"
#include "Common/World/Context.h"

namespace Mcc
{

	UserSessionModule::UserSessionModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "UserSessionModule require EntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerSessionModule...");
		world.module<UserSessionModule>();

		auto* ctx = static_cast<WorldContext*>(world.get_ctx());

		mLookupEntityQuery = world.query_builder<const Transform>().with<NetworkEntityTag>().build();
		mLookupBlockQuery  = world.query_builder<const BlockMeta, const BlockType>().with<BlockTag>().build();
		mLookupChunkQuery  = world.query_builder<const ChunkPosition, const ChunkData>().with<ChunkTag>().build();

		ctx->networkManager.Subscribe<ConnectEvent>   ([&](const auto& event) { OnConnectEventHandler   (world, event); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event) { OnDisconnectEventHandler(world, event); });
	}

	void UserSessionModule::OnConnectEventHandler(flecs::world& world, const ConnectEvent& event) const
	{
		auto* 		ctx = static_cast <WorldContext*>		   (world.get_ctx());
		const auto& net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		OnConnection packet;
		packet.initialState = {};

		mLookupEntityQuery
			.run([&](flecs::iter& it) {
				while (it.next())
				{
					auto t = it.field<const Transform>(0);

					for (auto i: it)
					{
						auto entity = it.entity(i);

						const auto idIt = ctx->localToNetwork.find(entity.id());
						if (idIt == ctx->localToNetwork.cend())
						{
							MCC_LOG_WARN("No network id associated to the local entity {}", entity.id());
							continue;
						}

						packet.initialState.entities.push_back({ idIt->second, t[i], {} });
					}
				}
			});

		mLookupBlockQuery
			.each([&](flecs::entity entity, const BlockMeta& meta, const BlockType type) {
				const auto idIt = ctx->localToNetwork.find(entity.id());
				if (idIt == ctx->localToNetwork.cend())
				{
					MCC_LOG_WARN("No network id associated to the local entity {}", entity.id());
					return;
				}

				packet.initialState.blocks.push_back({ idIt->second, meta, type });
			});

		mLookupChunkQuery
			.each([&](flecs::entity entity, const ChunkPosition& position, const ChunkData& data) {
				const auto idIt = ctx->localToNetwork.find(entity.id());
				if (idIt == ctx->localToNetwork.cend())
				{
					MCC_LOG_WARN("No network id associated to the local entity {}", entity.id());
					return;
				}

				Chunk::Palette palette;
				for (auto e: data.data->GetPalette())
				{
					auto eIt = ctx->localToNetwork.find(e);
					if (eIt != ctx->localToNetwork.cend())
					{
						palette.push_back(eIt->second);
					}
					else
					{
						MCC_LOG_WARN("Unable to retrieve network id for block({}) in chunk({})", e, idIt->second);
						return;
					}
				}

				packet.initialState.chunks.push_back({ idIt->second, position, { palette, data.data->GetMapping() } });
			});

		auto id		= GenerateNetworkID();
		auto entity = world.entity().is_a<UserEntityPrefab>();
		event.peer->data = new PlayerInfo { id };

		char hostname[100];
		enet_address_get_host_ip(&event.peer->address, hostname, 100);
		MCC_LOG_INFO("Connection opened on port {} with network id {} (from {})", event.peer->address.port, id, hostname);

		ctx->localToNetwork.emplace(entity.id(), id);
		ctx->networkToLocal.emplace(id, entity.id());

		packet.playerInfo = *static_cast<PlayerInfo*>(event.peer->data);
		packet.serverInfo = ctx->serverInfo;

		net.Send(event.peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		entity.add<EntityCreatedTag>();
	}

	void UserSessionModule::OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent& event)
	{
		const auto* ctx  = static_cast<WorldContext*>(world.get_ctx());
		auto* playerInfo = static_cast<PlayerInfo*>	 (event.peer->data);
		auto  pInfoCopy  = *playerInfo;
		const auto it 	 = ctx->networkToLocal.find(playerInfo->id);

		MCC_LOG_INFO("Connection closed on port {} with network id {}", event.peer->address.port, playerInfo->id);
		delete playerInfo;

		if (it == ctx->networkToLocal.cend())
		{
			MCC_LOG_WARN("The network id {} isn't associated to a local entity", pInfoCopy.id);
			return;
		}

		if (!world.is_alive(it->second))
		{
			MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", pInfoCopy.id);
			return;
		}

		world.entity(it->second).add<EntityDestroyedTag>();
	}

}