//
// Created by Alex Clorennec on 07/07/2025.
//


#include "Client/Module/Renderer/System.h"
#include "Client/Graphics/Common.h"
#include "Client/World/Context.h"

#include <glad/glad.h>

namespace Mcc
{

	void SetupRendererSystem(flecs::iter& it)
	{
		while(it.next()) {}
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// glEnable(GL_CULL_FACE);
	}

	void PollWindowEventSystem(flecs::iter& it)
	{
		while(it.next()) {}
		Window::PollEvents();
	}

	void CleanupFrameSystem(flecs::iter& it)
	{
		while(it.next()) {}
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glCheck(glClearColor(1., 1., 1., 1.));
	}

	void RenderFrameSystem(flecs::iter& it)
	{
		while(it.next()) {}
		static_cast<ClientWorldContext*>(it.world().get_ctx())->window.SwapBuffer();
		// Window::PollEvents();
	}

}