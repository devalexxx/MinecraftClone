//
// Created by Alex on 30/06/2025.
//

#include "Server/Module/TerrainReplication/System.h"
#include "Server/WorldContext.h"
#include "Common/Module/Network/Component.h"
#include "Server/Module/TerrainReplication/Component.h"
#include "Server/ServerNetworkManager.h"

#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	void BroadcastCreatedBlocks(flecs::iter& it)
	{
		const auto* ctx = ServerWorldContext::Get(it.world());

		while (it.next())
		{
			auto m = it.field<const BlockMeta>   (0);
			auto t = it.field<const BlockType>   (1);
			auto c = it.field<const BlockColor>  (2);
			auto n = it.field<const NetworkProps>(3);

			OnBlocksCreated packet;
			for (const auto i: it)
			{
			    const auto entity = it.entity(i);
			    const auto handle = n[i].handle;

			    if (!IsValid(handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        continue;
			    }

			    packet.blocks.push_back({ handle, m[i], c[i].color, t[i] });
			    entity.remove<BlockCreatedTag>();
			    MCC_LOG_INFO("Block({}) has been created and replicated", handle);
			}
			ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDirtyBlocks(flecs::iter& it)
	{
	    const auto* ctx = ServerWorldContext::Get(it.world());

		while (it.next())
		{
		    auto m = it.field<const BlockMeta>   (0);
		    auto t = it.field<const BlockType>   (1);
		    auto c = it.field<const BlockColor>  (2);
		    auto n = it.field<const NetworkProps>(3);

			OnBlocksUpdated packet;
			for (const auto i: it)
			{
			    const auto entity = it.entity(i);
                const auto handle = n[i].handle;

			    if (!IsValid(handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        continue;
			    }

				packet.blocks.push_back({ handle, m[i], c[i].color, t[i] });
				entity.remove<BlockDirtyTag>();
			}
			ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDestroyedBlocks(flecs::iter& it)
	{
        const auto* ctx = ServerWorldContext::Get(it.world());

		while (it.next())
		{
		    auto n = it.field<const NetworkProps>(0);

			OnBlocksDestroyed packet;
			for (const auto i: it)
			{
			    const auto entity = it.entity(i);
			    const auto handle = n[i].handle;

			    if (!IsValid(handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        entity.destruct();
			        continue;
			    }

				packet.handles.push_back(handle);
				entity.destruct();
			    MCC_LOG_INFO("Block({}) has been destroyed and replicated", handle);
			}
			ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastCreatedChunks(flecs::iter& it)
	{
		const auto* ctx   = ServerWorldContext::Get(it.world());
	    const auto& world = it.world();

		while (it.next())
		{
			auto t = it.field<const ChunkPosition>(0);
			auto h = it.field<const ChunkHolder>  (1);
		    auto n = it.field<const NetworkProps> (2);

			OnChunksCreated packet;
			for (const auto i: it)
			{
			    const auto entity = it.entity(i);
			    const auto handle = n[i].handle;

			    if (!IsValid(handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        continue;
			    }

				if (auto data = h[i].chunk->ToNetwork(world); data.has_value())
				{
					packet.chunks.push_back({ handle, t[i], std::move(*data) });
					MCC_LOG_INFO("Chunk({}) has been created and replicated", handle);
				}

			    entity.remove<ChunkCreatedTag>();
			}
			ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDirtyChunks(flecs::iter& it)
	{
		const auto* ctx   = ServerWorldContext::Get(it.world());
	    const auto& world = it.world();

		while (it.next())
		{
			auto t = it.field<const ChunkPosition>(0);
			auto h = it.field<const ChunkHolder>  (1);
		    auto n = it.field<const NetworkProps> (2);

			OnChunksUpdated packet;
			for (const auto i: it)
			{
			    const auto entity = it.entity(i);
			    const auto handle = n[i].handle;

			    if (!IsValid(handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        continue;
			    }

				if (auto data = h[i].chunk->ToNetwork(world); data.has_value())
				{
					packet.chunks.push_back({ handle, t[i], std::move(*data) });
				}

			    entity.remove<ChunkDirtyTag>();
			}
			ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDestroyedChunks(flecs::iter& it)
	{
        const auto* ctx = ServerWorldContext::Get(it.world());

		while (it.next())
		{
		    auto n = it.field<const NetworkProps>(0);

			OnChunksDestroyed packet;
			for (const auto i: it)
			{
			    const auto entity = it.entity(i);
			    const auto handle = n[i].handle;

			    if (!IsValid(handle))
			    {
			        MCC_LOG_WARN("The network id attached to #{} is invalid", entity.id());
			        entity.destruct();
			        continue;
			    }

			    packet.handles.push_back(handle);
				entity.destruct();
			    MCC_LOG_INFO("Chunk({}) has been destroyed and replicated", handle);
			}
			ctx->networkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

}