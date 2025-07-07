//
// Created by Alex on 22/09/2024.
//

#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/VertexArray.h"

#include <flecs.h>

namespace Mcc
{

	struct Renderer
	{
			Renderer(flecs::world& world);

			Mesh 		mCube;
			Program 	mProgram;
			VertexArray mVertexArray;

			void RenderEntitiesSystem(flecs::iter& it);
			void RenderTerrainSystem (flecs::iter& it);
	};

}

#endif
