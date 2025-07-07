//
// Created by Alex Clorennec on 07/07/2025.
//

#include "Client/Module/TerrainReplication/System.h"
#include "Client/Module/TerrainReplication/Component.h"

namespace Mcc
{

	void RemoveChunkDirtySystem(flecs::entity entity)
	{
		entity.remove<ChunkDirtyTag>();
	}


}