//
// Created by Alex on 01/07/2025.
//

#include "Client/Module/TerrainReplication/Module.h"
#include "Client/WorldContext.h"
#include "Client/Module/ServerSession/Component.h"
#include "Client/Module/TerrainReplication/Component.h"
#include "Client/Module/TerrainReplication/System.h"

#include "Common/Module/Terrain/Module.h"

#include "Common/Module/Network/Component.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"
#include "Common/World/Block.h"

namespace Mcc
{

	TerrainReplicationModule::TerrainReplicationModule(const flecs::world& world)
	{
		MCC_ASSERT	 (world.has<TerrainModule>(), "TerrainReplicationModule require TerrainModule, you must import it before.");
		MCC_LOG_DEBUG("Import TerrainReplication...");
		world.module<TerrainReplicationModule>();

		world.component<ChunkDirtyTag>();

		world.system("RemoveChunkDirty")
			.with<ChunkDirtyTag>()
			.each(RemoveChunkDirtySystem);

	    world.system<InitialWorldState>("DispatchInitialTerrainState")
            .kind(flecs::OnStart)
	        .immediate()
            .term_at(0).singleton()
            .each(DispatchInitialTerrainStateSystem);

        const auto* ctx = ClientWorldContext::Get(world);

		ctx->networkManager.Subscribe<OnBlocksCreated>([&world](const auto& event) { OnBlocksCreatedHandler(world, event); });
		ctx->networkManager.Subscribe<OnChunksCreated>([&world](const auto& event) { OnChunksCreatedHandler(world, event); });

		ctx->networkManager.Subscribe<OnBlocksUpdated>([&world](const auto& event) { OnBlocksUpdatedHandler(world, event); });
		ctx->networkManager.Subscribe<OnChunksUpdated>([&world](const auto& event) { OnChunksUpdatedHandler(world, event); });

		ctx->networkManager.Subscribe<OnChunksDestroyed>([&world](const auto& event) { OnChunksDestroyedHandler(world, event); });
		ctx->networkManager.Subscribe<OnBlocksDestroyed>([&world](const auto& event) { OnBlocksDestroyedHandler(world, event); });
	}

	void TerrainReplicationModule::OnBlocksCreatedHandler(const flecs::world& world, const OnBlocksCreated& event)
	{
        const auto* ctx = ClientWorldContext::Get(world);

		for (const auto& [handle, meta, type]: event.blocks)
		{
			if (ctx->networkMapping.GetLHandle(handle).has_value())
			{
				MCC_LOG_WARN("The network id {} is already associated to a local block", handle);
				continue;
			}

		    world.defer_suspend();

			world.entity()
                .is_a<BlockPrefab>()
		        .set<NetworkProps>({ handle })
                .set<BlockMeta>(meta)
		        .set<BlockType>(type);

		    world.defer_resume();
		}
	}

	void TerrainReplicationModule::OnChunksCreatedHandler(const flecs::world& world, const OnChunksCreated& event)
	{
        const auto* ctx = ClientWorldContext::Get(world);

	    for (const auto& [handle, position, data]: event.chunks)
	    {
	        if (ctx->networkMapping.GetLHandle(handle).has_value())
	        {
	            MCC_LOG_WARN("The network id {} is already associated to a local chunk", handle);
	            continue;
	        }

	        if (auto from = Helper::FromNetwork(data, world); from.has_value())
	        {
	            world.defer_suspend();

	            world.entity()
                    .is_a<ChunkPrefab>()
                    .add<ChunkDirtyTag>()
	                .set<NetworkProps>({ handle })
                    .set<ChunkPosition>(position)
                    .set<ChunkHolder>({ std::make_shared<Chunk>(std::move(*from)) });

	            world.defer_resume();
	        }
	    }
	}

	void TerrainReplicationModule::OnBlocksUpdatedHandler(const flecs::world& world, const OnBlocksUpdated& event)
	{
		const auto* ctx = ClientWorldContext::Get(world);

		for (auto& desc: event.blocks)
		{
		    if (auto id = ctx->networkMapping.GetLHandle(desc.handle); id.has_value())
		    {
		        if (!world.is_alive(*id))
		        {
		            MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", desc.handle);
		            continue;
		        }

		        // TODO
		    }
		}
	}

	void TerrainReplicationModule::OnChunksUpdatedHandler(const flecs::world& world, const OnChunksUpdated& event)
	{
		const auto* ctx = ClientWorldContext::Get(world);

		for (auto& desc: event.chunks)
		{
		    if (auto id = ctx->networkMapping.GetLHandle(desc.handle); id.has_value())
		    {
		        if (!world.is_alive(*id))
		        {
		            MCC_LOG_WARN("The network id {} isn't associated to a local chunk", desc.handle);
		            continue;
		        }

		        // TODO
		    }
		}
	}

	void TerrainReplicationModule::OnChunksDestroyedHandler(const flecs::world& world, const OnChunksDestroyed& event)
	{
        const auto* ctx = ClientWorldContext::Get(world);

		for (auto handle: event.handles)
		{
		    if (auto id = ctx->networkMapping.GetLHandle(handle); id.has_value())
		    {
		        if (!world.is_alive(*id))
		        {
		            MCC_LOG_WARN("The local chunk associated to the network id {} isn't alive", handle);
		            continue;
		        }

		        world.entity(*id).destruct();
		    }
		}
	}

	void TerrainReplicationModule::OnBlocksDestroyedHandler(const flecs::world& world, const OnBlocksDestroyed& event)
	{
        const auto* ctx = ClientWorldContext::Get(world);

		for (auto& handle: event.handles)
		{
		    if (auto id = ctx->networkMapping.GetLHandle(handle); id.has_value())
		    {
		        if (!world.is_alive(*id))
		        {
		            MCC_LOG_WARN("The local block associated to the network id {} isn't alive", handle);
		            continue;
		        }

		        world.entity(*id).destruct();
		    }
		}
	}

}