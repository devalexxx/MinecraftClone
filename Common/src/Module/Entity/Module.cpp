//
// Created by Alex on 28/06/2025.
//

#include "Common/Module/Entity/Module.h"
#include "Common/Module/Entity/Component.h"
#include "Common/Module/Network/Component.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	EntityModule::EntityModule(flecs::world& world)
	{
		MCC_LOG_DEBUG("Import EntityModule...");
		world.import<EntityModule>();

		world.component<EntityTag>();
		world.component<NetworkEntityTag>();
		world.component<UserEntityTag>();

		world.component<EntityPrefab>();
		world.component<NetworkEntityPrefab>();
		world.component<UserEntityPrefab>();

		world.prefab<EntityPrefab>()
		    .add<EntityTag>()
		    .set_auto_override<Transform>({{ 0.f, 0.f, 0.f }, {}, { 1.f, 1.f, 1.f }});

		world.prefab<NetworkEntityPrefab>()
	        .is_a<NetworkObjectPrefab>()
		    .is_a<EntityPrefab>()
			.add<NetworkEntityTag>()
			.set_auto_override<Extra>({});

		world.prefab<UserEntityPrefab>()
		    .is_a<NetworkEntityPrefab>()
			.add<UserEntityTag>()
			.set_auto_override<UserInputQueue>({});
	}

}