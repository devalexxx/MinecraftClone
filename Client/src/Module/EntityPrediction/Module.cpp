//
// Created by Alex on 25/06/2025.
//

#include "Client/Module/EntityPrediction/Module.h"
#include "Client/Module/EntityPrediction/System.h"
#include "Client/Module/EntityPrediction/Tag.h"

#include "Common/Module/WorldEntity/Module.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	EntityPredictionModule::EntityPredictionModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<WorldEntityModule>(), "EntityPredictionModule require WorldEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityPredictionModule...");
		world.module<EntityPredictionModule>();

		world.component<ExcludePredictionTag>();

		world.system<Transform, SnapshotQueue>().without<ExcludePredictionTag>().each(InterpolateEntityTransform);
	}

}