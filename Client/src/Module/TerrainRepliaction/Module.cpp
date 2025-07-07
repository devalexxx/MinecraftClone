//
// Created by Alex on 01/07/2025.
//

#include "Client/Module/TerrainReplication/Module.h"
#include "Client/Module/TerrainReplication/Component.h"
#include "Client/Module/TerrainReplication/System.h"
#include "Client/Module/ServerSession/Component.h"
#include "Client/World/Context.h"

#include "Common/Module/Terrain/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	TerrainReplicationModule::TerrainReplicationModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<TerrainModule>(), "TerrainReplicationModule require TerrainModule, you must import it before.");
		MCC_LOG_DEBUG("Import TerrainReplication...");
		world.module<TerrainReplicationModule>();

		world.component<ChunkDirtyTag>();

		world.system("RemoveChunkDirty")
			.with<ChunkDirtyTag>()
			.each(RemoveChunkDirtySystem);

		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnBlocksCreated>([&world](const auto& event) { OnBlocksCreatedHandler(world, event); });
		ctx->networkManager.Subscribe<OnChunksCreated>([&world](const auto& event) { OnChunksCreatedHandler(world, event); });

		ctx->networkManager.Subscribe<OnBlocksUpdated>([&world](const auto& event) { OnBlocksUpdatedHandler(world, event); });
		ctx->networkManager.Subscribe<OnChunksUpdated>([&world](const auto& event) { OnChunksUpdatedHandler(world, event); });

		ctx->networkManager.Subscribe<OnChunksDestroyed>([&world](const auto& event) { OnChunksDestroyedHandler(world, event); });
		ctx->networkManager.Subscribe<OnBlocksDestroyed>([&world](const auto& event) { OnBlocksDestroyedHandler(world, event); });

		MCC_LOG_DEBUG("Load initial world state...");
		const auto* initialState = world.try_get<InitialWorldState>();
		if (initialState)
		{
			for (auto& desc: initialState->state.blocks)
			{

				auto entity = world.entity()
					.add<BlockTag>()
					.set<BlockMeta>(desc.meta)
					.set<BlockType>(desc.type);

				ctx->localToNetwork.emplace(entity.id(), desc.id);
				ctx->networkToLocal.emplace(desc.id, entity.id());
			}

			for (auto& desc: initialState->state.chunks)
			{

				Chunk::Palette palette;
				bool 		   success = true;
				for (auto e: desc.data.GetPalette())
				{
					auto eIt = ctx->networkToLocal.find(e);
					if (eIt != ctx->networkToLocal.cend())
					{
						palette.push_back(eIt->second);
					}
					else
					{
						MCC_LOG_WARN("Unable to retrieve local id for block({}) in chunk({})", e, desc.id);
						success = false;
						break;
					}
				}

				if (success)
				{
					auto entity = world.entity()
						.add<ChunkTag>()
						.add<ChunkDirtyTag>()
						.set<ChunkPosition>(desc.position)
						.set<ChunkData>({ std::make_unique<Chunk>(palette, desc.data.GetMapping()) });

					ctx->localToNetwork.emplace(entity.id(), desc.id);
					ctx->networkToLocal.emplace(desc.id, entity.id());
				}
			}
		}
	}

	void TerrainReplicationModule::OnBlocksCreatedHandler(flecs::world& world, const OnBlocksCreated& event)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& desc: event.blocks)
		{
			auto it = ctx->networkToLocal.find(desc.id);
			if (it != ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} is already associated to a local block", desc.id);
				continue;
			}

			auto entity = world.entity()
				  .add<BlockTag>()
				  .set<BlockMeta>(desc.meta);

			ctx->localToNetwork.emplace(entity.id(), desc.id);
			ctx->networkToLocal.emplace(desc.id, entity.id());
		}
	}

	void TerrainReplicationModule::OnChunksCreatedHandler(flecs::world& world, const OnChunksCreated& event)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& desc: event.chunks)
		{
			auto it = ctx->networkToLocal.find(desc.id);
			if (it != ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} is already associated to a local chunk", desc.id);
				continue;
			}

			Chunk::Palette palette;
			bool 		   success = true;
			for (auto e: desc.data.GetPalette())
			{
				auto eIt = ctx->networkToLocal.find(e);
				if (eIt != ctx->networkToLocal.cend())
				{
					palette.push_back(eIt->second);
				}
				else
				{
					MCC_LOG_WARN("Unable to retrieve local id for block({}) in chunk({})", e, desc.id);
					success = false;
					break;
				}
			}

			if (success)
			{
				auto entity = world.entity()
					.add<ChunkTag>()
					.set<ChunkPosition>(desc.position)
					.set<ChunkData>({ std::make_unique<Chunk>(palette, desc.data.GetMapping()) });

				ctx->localToNetwork.emplace(entity.id(), desc.id);
				ctx->networkToLocal.emplace(desc.id, entity.id());
			}
		}
	}

	void TerrainReplicationModule::OnBlocksUpdatedHandler(flecs::world& world, const OnBlocksUpdated& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& desc: event.blocks)
		{
			auto it = ctx->networkToLocal.find(desc.id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} isn't associated to a local block", desc.id);
				continue;
			}

			if (!world.is_alive(it->second))
			{
				MCC_LOG_WARN("The local block associated to the network id {} isn't alive", desc.id);
				continue;
			}

			// TODO
		}
	}

	void TerrainReplicationModule::OnChunksUpdatedHandler(flecs::world& world, const OnChunksUpdated& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& desc: event.chunks)
		{
			auto it = ctx->networkToLocal.find(desc.id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} isn't associated to a local chunk", desc.id);
				continue;
			}

			if (!world.is_alive(it->second))
			{
				MCC_LOG_WARN("The local chunk associated to the network id {} isn't alive", desc.id);
				continue;
			}

			// TODO
		}
	}

	void TerrainReplicationModule::OnChunksDestroyedHandler(flecs::world& world, const OnChunksDestroyed& event)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& id: event.ids)
		{
			auto it = ctx->networkToLocal.find(id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} isn't associated to a local chunk", id);
				continue;
			}

			if (!world.is_alive(it->second))
			{
				MCC_LOG_WARN("The local chunk associated to the network id {} isn't alive", id);
				continue;
			}

			world.entity(it->second).destruct();

			ctx->localToNetwork.erase(it->second);
			ctx->networkToLocal.erase(id);
		}
	}

	void TerrainReplicationModule::OnBlocksDestroyedHandler(flecs::world& world, const OnBlocksDestroyed& event)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& id: event.ids)
		{
			auto it = ctx->networkToLocal.find(id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} isn't associated to a local block", id);
				continue;
			}

			if (!world.is_alive(it->second))
			{
				MCC_LOG_WARN("The local block associated to the network id {} isn't alive", id);
				continue;
			}

			world.entity(it->second).destruct();

			ctx->localToNetwork.erase(it->second);
			ctx->networkToLocal.erase(id);
		}
	}

}