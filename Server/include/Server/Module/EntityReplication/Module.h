//
// Created by Alex on 22/06/2025.
//

#ifndef MCC_MODULE_SERVER_WORLD_ENTITY_MODULE_H
#define MCC_MODULE_SERVER_WORLD_ENTITY_MODULE_H

#include <flecs.h>

namespace Mcc
{

	class EntityReplicationModule
	{
		public:
			EntityReplicationModule(flecs::world& world);
	};

}

#endif
