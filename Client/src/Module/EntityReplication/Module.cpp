//
// Created by Alex on 21/06/2025.
//

#include <Client/Module/EntityReplication/Module.h>
#include <Client/World/Context.h>

#include <MinecraftLib/Module/WorldEntity/Module.h>
#include <MinecraftLib/Module/WorldEntity/Prefab.h>
#include <MinecraftLib/Network/Packet.h>

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(flecs::world& world)
	{
		assert(world.has<WorldEntityModule>());

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
			world.entity(ctx->networkToLocal[id]).destruct();

			ctx->localToNetwork.erase(ctx->networkToLocal[id]);
			ctx->networkToLocal.erase(id);
		}
	}

	void EntityReplicationModule::OnEntitiesUpdatedHandler(flecs::world& world, const OnEntitiesUpdated& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		for (auto& state: event.states)
		{
			auto id = ctx->networkToLocal.find(state.id)->second;
			if (world.exists(id))
			{
				world.entity(id)
					.set(state.position)
					.set(state.rotation)
					.set(Forward::FromRotation(state.rotation))
					.set(Right  ::FromRotation(state.rotation));
			}
		}
	}

}