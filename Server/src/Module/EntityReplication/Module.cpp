//
// Created by Alex on 22/06/2025.
//

#include "Server/Module/EntityReplication/Module.h"
#include "Server/Module/EntityReplication/System.h"

#include "Common/Module/WorldEntity/Module.h"
#include "Common/Utils/Logging.h"
#include "Common/Utils/Assert.h"

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<WorldEntityModule>(), "EntityReplicationModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityReplicationModule...");
		world.module<EntityReplicationModule>();

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