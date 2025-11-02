//
// Created by Alex on 30/06/2025.
//

#ifndef MCC_SERVER_MODULE_TERRAIN_MODULE_H
#define MCC_SERVER_MODULE_TERRAIN_MODULE_H

#include "Server/Module/UserSession/Module.h"

#include <flecs.h>

namespace Mcc
{

	struct TerrainReplicationModule
	{
		TerrainReplicationModule(const flecs::world& world);

	    static void ReplicateChunk(UserSession* session, const flecs::world& world, flecs::entity_t chunk);
	};

}

#endif
