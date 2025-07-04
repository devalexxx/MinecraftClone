//
// Created by Alex on 22/09/2024.
//

#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/VertexArray.h"
#include "Client/Module/Camera/Component.h"
#include "Client/World/Context.h"

#include "Common/Module/Entity/Component.h"

#include <flecs.h>

namespace Mcc
{

	struct Renderer
	{
			Renderer(flecs::world& world);

			flecs::query<const Transform, const CameraSettings> mCameraQuery;

			Mesh 		mCube;
			Program 	mProgram;
			VertexArray mVertexArray;

			std::pair<glm::mat4, glm::mat4> GetCamera(const ClientWorldContext* ctx) const;
			void RenderEntitiesSystem(flecs::iter& it);
			void RenderTerrainSystem (flecs::iter& it);
	};

}

#endif
