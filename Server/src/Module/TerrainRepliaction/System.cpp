//
// Created by Alex on 30/06/2025.
//

#include "Server/Module/TerrainReplication/System.h"
#include "Server/Module/TerrainReplication/Component.h"
#include "Server/ServerNetworkManager.h"
#include "Server/World/Context.h"

#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	void BroadcastCreatedBlocks(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto m = it.field<const BlockMeta>(0);
			auto t = it.field<const BlockType>(1);

			OnBlocksCreated packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.blocks.push_back({ id->second, m[i], t[i] });
					MCC_LOG_INFO("Block({}) has been created and replicated", id->second);
					entity.remove<BlockCreatedTag>();
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for entity({})", entity.id());
				}
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDirtyBlocks(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto m = it.field<const BlockMeta>(0);
			auto t = it.field<const BlockType>(1);

			OnBlocksUpdated packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.blocks.push_back({ id->second, m[i], t[i] });
					entity.remove<BlockDirtyTag>();
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for block({})", entity.id());
				}
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDestroyedBlocks(flecs::iter& it)
	{
		auto* 		ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto& net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			OnBlocksDestroyed packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.ids.push_back(id->second);

					MCC_LOG_INFO("Block({}) has been destroyed and replicated", id->second);

					ctx->networkToLocal.erase(id->second);
					ctx->localToNetwork.erase(id->first);
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for block({})", entity.id());
				}

				entity.destruct();
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastCreatedChunks(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto t = it.field<const ChunkPosition>(0);
			auto d = it.field<const ChunkData>	  (1);

			OnChunksCreated packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					Chunk::Palette palette;
					bool 		   success = true;
					for (auto e: d[i].data->GetPalette())
					{
						auto eIt = ctx->localToNetwork.find(e);
						if (eIt != ctx->localToNetwork.cend())
						{
							palette.push_back(eIt->second);
						}
						else
						{
							MCC_LOG_WARN("Unable to retrieve network id for block({}) in chunk({})", e, id->second);
							success = false;
							break;
						}
					}

					if (success)
					{
						packet.chunks.push_back({ id->second, t[i], { palette, d[i].data->GetMapping() } });
						MCC_LOG_INFO("Chunk({}) has been created and replicated", id->second);
						entity.remove<ChunkCreatedTag>();
					}
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for chunk({})", entity.id());
				}
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDirtyChunks(flecs::iter& it)
	{
		const auto* ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto&	net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			auto t = it.field<const ChunkPosition>(0);
			auto d = it.field<const ChunkData>	  (1);

			OnChunksUpdated packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					Chunk::Palette palette;
					bool 		   success = true;
					for (auto e: d[i].data->GetPalette())
					{
						auto eIt = ctx->localToNetwork.find(e);
						if (eIt != ctx->localToNetwork.cend())
						{
							palette.push_back(eIt->second);
						}
						else
						{
							MCC_LOG_WARN("Unable to retrieve network id for block({}) in chunk({})", e, id->second);
							success = false;
							break;
						}
					}

					if (success)
					{
						packet.chunks.push_back({ id->second, t[i], { palette, d[i].data->GetMapping() } });
						entity.remove<ChunkDirtyTag>();
					}
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for chunk({})", entity.id());
				}
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

	void BroadcastDestroyedChunks(flecs::iter& it)
	{
		auto* 		ctx = static_cast <ServerWorldContext*>  (it.world().get_ctx());
		const auto& net = dynamic_cast<ServerNetworkManager&>(ctx->networkManager);

		while (it.next())
		{
			OnChunksDestroyed packet;
			for (auto i: it)
			{
				auto entity = it.entity(i);
				if (auto id = ctx->localToNetwork.find(entity.id()); id != ctx->localToNetwork.cend())
				{
					packet.ids.push_back(id->second);

					MCC_LOG_INFO("Chunk({}) has been destroyed and replicated", id->second);

					ctx->networkToLocal.erase(id->second);
					ctx->localToNetwork.erase(id->first);
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve network id for chunk({})", entity.id());
				}

				entity.destruct();
			}
			net.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
		}
	}

}