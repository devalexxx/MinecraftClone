//
// Created by Alex on 30/06/2025.
//

#ifndef MCC_SERVER_MODULE_TERRAIN_SYSTEM_H
#define MCC_SERVER_MODULE_TERRAIN_SYSTEM_H

#include <flecs.h>

namespace Mcc
{

	void BroadcastCreatedBlocks  (flecs::iter& it);
	void BroadcastDirtyBlocks  	 (flecs::iter& it);
	void BroadcastDestroyedBlocks(flecs::iter& it);

	void BroadcastCreatedChunks  (flecs::iter& it);
	void BroadcastDirtyChunks  	 (flecs::iter& it);
	void BroadcastDestroyedChunks(flecs::iter& it);

}

#endif