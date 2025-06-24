//
// Created by Alex on 22/06/2025.
//

#include <Server/Module/EntityReplication/Module.h>
#include <Server/Module/EntityReplication/System.h>

#include <MinecraftLib/Module/WorldEntity/Module.h>

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(flecs::world& world)
	{
		assert(world.has<WorldEntityModule>());

		world.system<const Position, const Rotation, const WorldEntityExtra>()
			.with<WorldEntityCreatedTag>()
			.run(BroadcastEntitiesCreated);

		world.system<const Position, const Rotation, const WorldEntityExtra>()
		    .with<WorldEntityUpdatedTag>()
			.run(BroadcastEntitiesUpdated);

		world.system()
			.with<WorldEntityDestroyedTag>()
			.run(BroadcastEntitiesDestroyed);
	}

}