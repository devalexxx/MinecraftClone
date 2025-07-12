//
// Created by Alex on 21/06/2025.
//

#include "Client/Module/EntityReplication/Module.h"
#include "Client/WorldContext.h"
#include "Client/Module/EntityReplication/Component.h"
#include "Client/Module/EntityReplication/System.h"
#include "Client/Module/ServerSession/Component.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"

#include "Common/Module/Network/Component.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(const flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "EntityReplicationModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityReplicationModule...");
		world.module<EntityReplicationModule>();

		world.component<InterpolationExcludedTag>();

		world.system<Transform, SnapshotQueue>("EntityInterpolation")
	        .without<InterpolationExcludedTag>()
	        .each(EntityInterpolationSystem);

		const auto* ctx = ClientWorldContext::Get(world);
		ctx->networkManager.Subscribe<OnEntitiesCreated>  ([&world](const auto& event) { OnEntitiesCreatedHandler  (world, event); });
		ctx->networkManager.Subscribe<OnEntitiesUpdated>  ([&world](const auto& event) { OnEntitiesUpdatedHandler  (world, event); });
		ctx->networkManager.Subscribe<OnEntitiesDestroyed>([&world](const auto& event) { OnEntitiesDestroyedHandler(world, event); });
	}

	void EntityReplicationModule::OnEntitiesCreatedHandler(const flecs::world& world, const OnEntitiesCreated& event)
	{
        const auto* ctx = ClientWorldContext::Get(world);

		for (auto& state: event.states)
		{
			if (ctx->networkMapping.GetLHandle(state.handle).has_value())
			{
				MCC_LOG_WARN("The network id {} is already associated to a local entity", state.handle);
				continue;
			}

			world.entity()
			  .is_a<NetworkEntityPrefab>()
			  .set<NetworkProps>({ state.handle })
			  .set(state.transform)
			  .set<SnapshotQueue>({});
		}
	}

	void EntityReplicationModule::OnEntitiesDestroyedHandler(const flecs::world& world, const OnEntitiesDestroyed& event)
	{
        const auto* ctx = ClientWorldContext::Get(world);

		for (const auto handle: event.handles)
		{
		    if (auto id = ctx->networkMapping.GetLHandle(handle); id.has_value())
		    {
		        if (!world.is_alive(*id))
		        {
		            MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", handle);
		            continue;
		        }

		        world.entity(*id).destruct();
		    }
		}
	}

	void EntityReplicationModule::OnEntitiesUpdatedHandler(const flecs::world& world, const OnEntitiesUpdated& event)
	{
		const auto* ctx = ClientWorldContext::Get(world);

		for (const auto& state: event.states)
		{
		    if (auto id = ctx->networkMapping.GetLHandle(state.handle); id.has_value())
		    {
		        if (!world.is_alive(*id))
		        {
		            MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", state.handle);
		            continue;
		        }

		        world.entity(*id).get([&state](SnapshotQueue& queue) {
		            queue.data.push_front({ state.transform, TimeClock::now() });
		        });
		    }
		}
	}

}