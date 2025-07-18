//
// Created by Alex on 22/06/2025.
//

#include "Server/Module/EntityReplication/Module.h"
#include "Server/Module/EntityReplication/System.h"
#include "Server/Module/EntityReplication/Component.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Module/Network/Component.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	EntityReplicationModule::EntityReplicationModule(const flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityModule>(), "EntityReplicationModule require EntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityReplicationModule...");
		world.module<EntityReplicationModule>();

		world.component<EntityDirtyTag>();

		world.system<const Transform, const Extra, const NetworkProps>()
			.with<EntityCreatedTag>()
			.run(BroadcastEntitiesCreated);

		world.system<const Transform, const Extra, const NetworkProps>()
		    .with<EntityDirtyTag>()
			.run(BroadcastEntitiesUpdated);

		world.system<const NetworkProps>()
			.with<EntityDestroyedTag>()
			.run(BroadcastEntitiesDestroyed);
	}

}