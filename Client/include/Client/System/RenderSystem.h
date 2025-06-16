//
// Created by Alex on 22/09/2024.
//

#ifndef MINECRAFT_RENDERSYSTEM_H
#define MINECRAFT_RENDERSYSTEM_H

#include <Client/Graphics/Mesh.h>
#include <Client/Graphics/Program.h>
#include <Client/Graphics/VertexArray.h>

namespace flecs
{
	struct world;
	struct iter;
}

namespace Mcc
{

	struct PlayerInfo;
	class Window;

	class RenderSystem
	{
		public:
			RenderSystem(flecs::world& world, const PlayerInfo& info, Window& window);

		private:
			void SetupRender(flecs::iter&);
			void SetupCamera(flecs::iter&);
			void RenderWorld(flecs::iter&);
			void UpdateWindow(flecs::iter&);

		private:
			flecs::world&     mWorld;
			const PlayerInfo& mInfo;
			Window&			  mWindow;

			Mesh mCube;
			Program mProgram;
			VertexArray mVertexArray;
			glm::mat4   mProjection;
	};

}

#endif
