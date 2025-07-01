//
// Created by Alex on 21/06/2025.
//

#include "Client/Module/EntityReplication/Module.h"
#include "Client/Module/EntityReplication/System.h"
#include "Client/Module/EntityReplication/Component.h"
#include "Client/Module/ServerSession/Component.h"
#include "Client/World/Context.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "EntityReplicationModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityReplicationModule...");
		world.module<EntityReplicationModule>();

		world.component<InterpolationExcludedTag>();

		world.system<Transform, SnapshotQueue>("EntityInterpolationSystem").without<InterpolationExcludedTag>().each(EntityInterpolationSystem);

		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnEntitiesCreated>  ([&world](const auto& event) { OnEntitiesCreatedHandler  (world, event); });
		ctx->networkManager.Subscribe<OnEntitiesUpdated>  ([&world](const auto& event) { OnEntitiesUpdatedHandler  (world, event); });
		ctx->networkManager.Subscribe<OnEntitiesDestroyed>([&world](const auto& event) { OnEntitiesDestroyedHandler(world, event); });

		MCC_LOG_DEBUG("Load initial world state...");
		const auto* initialState = world.get<InitialWorldState>();
		if (initialState)
		{
			for (auto& state: initialState->state.entities)
			{
				auto entity = world.entity()
					.is_a<NetworkEntityPrefab>()
					.set<NetworkHandle>({ state.id })
					.set(state.transform)
					.set<SnapshotQueue>({});

				ctx->localToNetwork.emplace(entity.id(), state.id);
				ctx->networkToLocal.emplace(state.id, entity.id());
			}
		}
	}

	void EntityReplicationModule::OnEntitiesCreatedHandler(flecs::world& world, const OnEntitiesCreated& event)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& state: event.states)
		{
			auto it = ctx->networkToLocal.find(state.id);
			if (it != ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} is already associated to a local entity", state.id);
				continue;
			}

			auto entity = world.entity()
			  .is_a<NetworkEntityPrefab>()
			  .set<NetworkHandle>({ state.id })
			  .set(state.transform)
			  .set<SnapshotQueue>({});

			ctx->localToNetwork.emplace(entity.id(), state.id);
			ctx->networkToLocal.emplace(state.id, entity.id());
		}
	}

	void EntityReplicationModule::OnEntitiesDestroyedHandler(flecs::world& world, const OnEntitiesDestroyed& event)
	{
		auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& id: event.ids)
		{
			auto it = ctx->networkToLocal.find(id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} isn't associated to a local entity", id);
				continue;
			}

			if (!world.is_alive(it->second))
			{
				MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", id);
				continue;
			}

			world.entity(it->second).destruct();

			ctx->localToNetwork.erase(it->second);
			ctx->networkToLocal.erase(id);
		}
	}

	void EntityReplicationModule::OnEntitiesUpdatedHandler(flecs::world& world, const OnEntitiesUpdated& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& state: event.states)
		{
			auto it = ctx->networkToLocal.find(state.id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("The network id {} isn't associated to a local entity", state.id);
				continue;
			}

			if (!world.is_alive(it->second))
			{
				MCC_LOG_WARN("The local entity associated to the network id {} isn't alive", state.id);
				continue;
			}

			world.entity(it->second).get([&state](SnapshotQueue& queue) {
				queue.data.push_front({ state.transform, TimeClock::now() });
			});
		}
	}

}