//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/UserSession/Module.h"
#include "Server/Module/EntityReplication/Component.h"
#include "Server/ServerNetworkManager.h"
#include "Server/WorldContext.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Benchmark.h"
#include "Common/Utils/Logging.h"
#include "Common/WorldContext.h"

namespace Mcc
{

    namespace _
    {

        void SendEntitiesTo(ENetPeer* peer, const ServerWorldContext* ctx, const UserSessionModule::EntityQuery& query)
        {
            OnEntitiesCreated packet;
            query.run([&packet](flecs::iter& it) {
                while (it.next())
                {
                    auto t = it.field<const Transform>   (0);
                    auto p = it.field<const NetworkProps>(1);

                    for (const auto i: it)
                    {
                        if (!IsValid(p[i].handle))
                        {
                            MCC_LOG_WARN("The network id attached to #{} is invalid", it.entity(i).id());
                            return;
                        }

                        packet.states.push_back({ p[i].handle, t[i], {} });
                    }
                }
            });
            ctx->networkManager.Send(peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
        }

        void SendBlocksTo(ENetPeer* peer, const ServerWorldContext* ctx, const UserSessionModule::BlockQuery& query)
        {
            OnBlocksCreated packet;
            query.run([&packet](flecs::iter& it) {
                while (it.next())
                {
                    auto m = it.field<const BlockMeta>   (0);
                    auto t = it.field<const BlockType>   (1);
                    auto c = it.field<const BlockColor>  (2);
                    auto p = it.field<const NetworkProps>(3);

                    for (const auto i: it)
                    {
                        if (!IsValid(p[i].handle))
                        {
                            MCC_LOG_WARN("The network id attached to #{} is invalid", it.entity(i).id());
                            return;
                        }

                        packet.blocks.push_back({ p[i].handle, m[i], c[i].color, t[i] });
                    }
                }
            });
            ctx->networkManager.Send(peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
        }

        void SendChunksTo(ENetPeer* peer, const ServerWorldContext* ctx, const UserSessionModule::ChunkQuery& query)
        {
            OnChunksCreated packet;
            query.run([ctx, &packet](flecs::iter& it) {
                using RType = std::optional<decltype(packet.chunks)::value_type>;
                std::deque<std::future<RType>> futures;
                while (it.next())
                {
                    auto t = it.field<const ChunkPosition>(0);
                    auto h = it.field<const ChunkHolder>  (1);
                    auto p = it.field<const NetworkProps> (2);

                    for (const auto i: it)
                    {
                        if (!IsValid(p[i].handle))
                        {
                            MCC_LOG_WARN("The network id attached to #{} is invalid", it.entity(i).id());
                            return;
                        }

                        futures.push_back(ctx->threadPool.ExecuteTask([i, &h, &t, &p, &it] -> RType {
                            if (auto data = MCC_BENCH_TIME(RLECompression, [&] { return h[i].chunk->ToNetwork(it.world()); })(); data.has_value())
                            {
                                return RType {{ p[i].handle, t[i], std::move(*data) }};
                            }
                            return std::nullopt;
                        }));
                    }

                    while (!futures.empty())
                    {
                        if (futures.front().wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                        {
                            if (auto v = futures.front().get(); v.has_value())
                            {
                                packet.chunks.push_back(std::move(*v));
                            }
                            futures.pop_front();
                        }
                    }
                }
            });
            ctx->networkManager.Send(peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
        }

    }

    UserSession* UserSession::Get(const ENetPeer* peer)
    {
        return static_cast<UserSession*>(peer->data);
    }

	UserSessionModule::UserSessionModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "UserSessionModule require EntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerSessionModule...");
		world.module<UserSessionModule>();

		mLookupEntityQuery = world.query_builder<const Transform, const NetworkProps>().with<NetworkEntityTag>().without<EntityCreatedTag>().build();
		mLookupBlockQuery  = world.query_builder<const BlockMeta, const BlockType, const BlockColor, const NetworkProps>().with<BlockTag>().build();
		mLookupChunkQuery  = world.query_builder<const ChunkPosition, const ChunkHolder, const NetworkProps>().with<ChunkTag>().build();

        const auto* ctx = WorldContext<>::Get(world);

		ctx->networkManager.Subscribe<ConnectEvent>   ([&](const auto& event) { OnConnectEventHandler   (world, event); });
		ctx->networkManager.Subscribe<DisconnectEvent>([&](const auto& event) { OnDisconnectEventHandler(world, event); });

		ctx->networkManager.Subscribe<From<OnClientInfo>>([&](const auto& from) { OnClientInfoHandler(world, from ); });
	}

	void UserSessionModule::OnConnectEventHandler(const flecs::world& world, const ConnectEvent& event)
    {
        const auto* ctx = ServerWorldContext::Get(world);

        auto handle = GenerateNetworkHandle();
        event.peer->data = new UserSession { { handle }, {} };

        char hostname[100];
        enet_address_get_host_ip(&event.peer->address, hostname, 100);
        MCC_LOG_INFO("Connection opened on port {} with network id {} (from {})", event.peer->address.port, handle, hostname);

        ctx->networkManager.Send<OnWaitingInfo>(event.peer, {}, ENET_PACKET_FLAG_RELIABLE, 0);
    }

    void UserSessionModule::OnClientInfoHandler(const flecs::world& world, const From<OnClientInfo>& from) const
    {
        const auto* ctx = ServerWorldContext::Get(world);
        if (auto* session = UserSession::Get(from.peer))
        {
            const OnConnectionAccepted packet { session->pInfo, ctx->settings };
            session->cInfo = from.packet.info;

            ctx->networkManager.Send(from.peer, packet, ENET_PACKET_FLAG_RELIABLE, 0);

            ctx->threadPool.ExecuteTask(MCC_BENCH_TIME(AA, _::SendEntitiesTo), from.peer, ctx, mLookupEntityQuery);
            ctx->threadPool.ExecuteTask(MCC_BENCH_TIME(BB, _::SendBlocksTo)  , from.peer, ctx, mLookupBlockQuery);
            ctx->threadPool.ExecuteTask(MCC_BENCH_TIME(CC, _::SendChunksTo)  , from.peer, ctx, mLookupChunkQuery);

            world.entity()
                .is_a<UserEntityPrefab>()
                .set<NetworkProps>({ session->pInfo.handle })
                .set<Transform>({ { 0, 100, 0 }, {}, { 1, 1, 1 } })
                .add<EntityCreatedTag>();
        }
        else
        {
            ctx->networkManager.Send<OnConnectionRefused>(from.peer, { "No user session found" }, ENET_PACKET_FLAG_RELIABLE, 0);
        }
    }

    void UserSessionModule::OnDisconnectEventHandler(const flecs::world& world, const DisconnectEvent& event)
	{
		const auto* ctx = WorldContext<>::Get(world);
        if (const auto* session = UserSession::Get(event.peer))
        {
            const auto  rHandle = session->pInfo.handle;
            const auto  lHandle = ctx->networkMapping.GetLHandle(rHandle);

            MCC_LOG_INFO("Connection closed on port {} with network id {}", event.peer->address.port, rHandle);
            delete session;

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

}