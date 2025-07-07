//
// Created by Alex Clorennec on 07/07/2025.
//

#ifndef MCC_CLIENT_MODULE_RENDERER_SYSTEM_H
#define MCC_CLIENT_MODULE_RENDERER_SYSTEM_H

#include <flecs.h>

namespace Mcc
{

	void SetupRendererSystem  (flecs::iter& it);
	void PollWindowEventSystem(flecs::iter& it);
	void CleanupFrameSystem	  (flecs::iter& it);
	void RenderFrameSystem	  (flecs::iter& it);

}

#endif
