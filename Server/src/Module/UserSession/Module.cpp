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
#include "Common/WorldContext.h"
#include "Server/WorldContext.h"

namespace Mcc
{

	UserSessionModule::UserSessionModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "UserSessionModule require EntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerSessionModule...");
		world.module<UserSessionModule>();

		mLookupEntityQuery = world.query_builder<const Transform, const NetworkProps>().with<NetworkEntityTag>().build();
		mLookupBlockQuery  = world.query_builder<const BlockMeta, const BlockType, const NetworkProps>().with<BlockTag>().build();
		mLookupChunkQuery  = world.query_builder<const ChunkPosition, const ChunkHolder, const NetworkProps>().with<ChunkTag>().build();

        const auto* ctx = WorldContext<>::Get(world);

		ctx->networkManager.Subscribe<ConnectEvent>   ([&](const auto& event) { OnConnectEventHandler   (world, event); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event) { OnDisconnectEventHandler(world, event); });
	}

	void UserSessionModule::OnConnectEventHandler(const flecs::world& world, const ConnectEvent& event) const
	{
		auto* ctx = ServerWorldContext::Get(world);

		OnConnection packet;
		packet.initialState = {};

		mLookupEntityQuery
			.each([&](const flecs::entity entity, const Transform& transform, const NetworkProps& props) {
			    if (!IsValid(props.handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        return;
			    }

				packet.initialState.entities.push_back({ props.handle, transform, {} });
			});

		mLookupBlockQuery
			.each([&](const flecs::entity entity, const BlockMeta& meta, const BlockType type, const NetworkProps& props) {
			    if (!IsValid(props.handle))
			    {
                    MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
                    return;
                }

				packet.initialState.blocks.push_back({ props.handle, meta, type });
			});

		mLookupChunkQuery
			.each([&](const flecs::entity entity, const ChunkPosition& position, const ChunkHolder& holder, const NetworkProps& props) {
			    if (!IsValid(props.handle))
			    {
                    MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
                    return;
                }

			    if (auto data = holder.chunk->ToNetwork(world); data.has_value())
			    {
                    packet.initialState.chunks.push_back({ props.handle, position, std::move(*data) });
                }
			});

        const auto entity = world.entity()
	        .is_a<UserEntityPrefab>();

	    NetworkHandle handle;
	    entity.get([&handle](const NetworkProps& props) {
	        handle = props.handle;
	    });

	    event.peer->data = new PlayerInfo { handle };

		char hostname[100];
		enet_address_get_host_ip(&event.peer->address, hostname, 100);
		MCC_LOG_INFO("Connection opened on port {} with network id {} (from {})", event.peer->address.port, handle, hostname);

	    ctx->networkMapping.Set(handle, entity.id());

		packet.playerInfo = *static_cast<PlayerInfo*>(event.peer->data);
		packet.serverInfo = ctx->serverInfo;

		ctx->networkManager.Send(event.peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		entity.add<EntityCreatedTag>();
	}

	void UserSessionModule::OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent& event)
	{
		const auto* ctx    = WorldContext<>::Get(world);
		const auto* info   = static_cast<PlayerInfo*>(event.peer->data);
		const auto rHandle = info->handle;
		const auto lHandle = ctx->networkMapping.GetLHandle(info->handle);

		MCC_LOG_INFO("Connection closed on port {} with network id {}", event.peer->address.port, rHandle);
		delete info;

		if (!lHandle.has_value())
		{
			MCC_LOG_WARN("The network id {} isn't associated to a local entity", rHandle);
			return;
		}

		if (!world.is_alive(*lHandle))
		{
			MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", rHandle);
			return;
		}

		world.entity(*lHandle).add<EntityDestroyedTag>();
	}

}