//
// Created by Alex on 21/06/2025.
//

#include "Common/Module/PlayerEntity/Module.h"
#include "Common/Module/WorldEntity/Module.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	WorldEntityModule::WorldEntityModule(flecs::world& world)
	{
		MCC_LOG_DEBUG("Import WorldEntityModule...");
		world.module<WorldEntityModule>();

		world.component<WorldEntityTag>();
		world.component<WorldEntityCreatedTag>();
		world.component<WorldEntityUpdatedTag>();
		world.component<WorldEntityDestroyedTag>();
		world.component<WorldEntityExtra>();
		world.component<Transform>();

		world.prefab<WorldEntityPrefab>()
			.add<WorldEntityTag>()
			.set_auto_override<WorldEntityExtra>({})
			.set_auto_override<SnapshotQueue>({})
			.set_auto_override<Transform>({
				{ 0.f, 0.f, 0.f }, {{0.f, 0.f, 0.f }}, { 1.f, 1.f, 1.f }
			});
	}

}