//
// Created by Alex Clorennec on 06/07/2025.
//

#ifndef MCC_CLIENT_MODULE_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_RENDERER_MODULE_H

#include <flecs.h>
#include <glm/fwd.hpp>

namespace Mcc
{

	struct RendererModule
	{
			RendererModule(flecs::world& world);

			static std::tuple<glm::vec3, glm::mat4, glm::mat4> GetView(const flecs::world& world);
	};

}

#endif
