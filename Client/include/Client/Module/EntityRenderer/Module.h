//
// Created by Alex Clorennec on 08/07/2025.
//

#ifndef MCC_CLIENT_MODULE_ENTITY_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_ENTITY_RENDERER_MODULE_H

#include "Client/Graphics/Buffer.h"
#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/VertexArray.h"

#include <flecs.h>

#include <glm/ext/scalar_constants.hpp>

namespace Mcc
{

	class EntityRendererModule
	{
		public:
			EntityRendererModule(const flecs::world& world);

			void SetupEntityMeshSystem (flecs::iter& it);
			void RenderUserEntitySystem(flecs::iter& it);

		private:
			VertexArray mVertexArray;
			Buffer      mVertexBuffer;
			Buffer      mIndexBuffer;
			size_t      mIndexCount;
			Program     mProgram;
	};
}

#endif
