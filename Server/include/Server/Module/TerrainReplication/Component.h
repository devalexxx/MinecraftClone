//
// Created by Alex on 30/06/2025.
//

#ifndef MCC_SERVER_MODULE_TERRAIN_COMPONENT_H
#define MCC_SERVER_MODULE_TERRAIN_COMPONENT_H

namespace Mcc
{

	struct ChunkDirtyTag     {};
	struct ChunkCreatedTag   {};
	struct ChunkDestroyedTag {};

	struct BlockDirtyTag     {};
	struct BlockCreatedTag   {};
	struct BlockDestroyedTag {};

}

#endif