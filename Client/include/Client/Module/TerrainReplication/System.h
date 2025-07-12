//
// Created by Alex Clorennec on 07/07/2025.
//

#ifndef MCC_CLIENT_MODULE_TERRAIN_REPLICATION_SYSTEM_H
#define MCC_CLIENT_MODULE_TERRAIN_REPLICATION_SYSTEM_H

#include <flecs.h>

namespace Mcc
{

	void RemoveChunkDirtySystem(flecs::entity entity);

}

#endif
