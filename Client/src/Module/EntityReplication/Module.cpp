//
// Created by Alex on 21/06/2025.
//

#include <Client/Module/EntityReplication/Module.h>
#include <Client/World/Context.h>

#include "MinecraftLib/Utils/Assert.h"
#include "MinecraftLib/Utils/Logging.h"
#include <MinecraftLib/Module/WorldEntity/Module.h>
#include <MinecraftLib/Module/WorldEntity/Prefab.h>
#include <MinecraftLib/Network/Packet.h>

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<WorldEntityModule>(), "EntityReplicationModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityReplicationModule...");
		world.module<EntityReplicationModule>();

		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnEntitiesCreated>  ([&world](const auto& event) { OnEntitiesCreatedHandler  (world, event); });
		ctx->networkManager.Subscribe<OnEntitiesUpdated>  ([&world](const auto& event) { OnEntitiesUpdatedHandler  (world, event); });
		ctx->networkManager.Subscribe<OnEntitiesDestroyed>([&world](const auto& event) { OnEntitiesDestroyedHandler(world, event); });
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
			  .is_a<WorldEntityPrefab>()
			  .set(state.position)
			  .set(state.rotation)
			  .set(Forward::FromRotation(state.rotation))
			  .set(Right  ::FromRotation(state.rotation));

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

			world.entity(it->second)
				.set(state.position)
				.set(state.rotation)
				.set(Forward::FromRotation(state.rotation))
				.set(Right  ::FromRotation(state.rotation));
		}
	}

}