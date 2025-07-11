//
// Created by Alex on 21/06/2025.
//

#ifndef MCC_CLIENT_MODULE_ENTITY_REPLICATION_MODULE_H
#define MCC_CLIENT_MODULE_ENTITY_REPLICATION_MODULE_H

#include <flecs.h>

namespace Mcc
{

	struct OnEntitiesCreated;
	struct OnEntitiesDestroyed;
	struct OnEntitiesUpdated;

	class EntityReplicationModule
	{
		public:
			EntityReplicationModule(const flecs::world& world);

		private:
			static void OnEntitiesCreatedHandler  (const flecs::world& world, const OnEntitiesCreated  & event);
			static void OnEntitiesDestroyedHandler(const flecs::world& world, const OnEntitiesDestroyed& event);
			static void OnEntitiesUpdatedHandler  (const flecs::world& world, const OnEntitiesUpdated  & event);
	};

}

#endif
