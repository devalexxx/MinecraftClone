//
// Created by Alex Clorennec on 08/07/2025.
//

#include "Client/Module/EntityRenderer/Module.h"
#include "Client/Module/Renderer/Module.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Utils/Assert.h"

namespace Mcc
{

	EntityRendererModule::EntityRendererModule(flecs::world& world) :
		mVertexBuffer(GL_ARRAY_BUFFER),
		mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER)
	{
		MCC_ASSERT(world.has<RendererModule>(), "EntityRendererModule require RendererModule, you must import it before.");
		MCC_LOG_DEBUG("Import EntityRendererModule...");
		world.module<EntityRendererModule>();

		world.system("SetupEntityMesh")
			.kind(flecs::OnStart)
			.run([this](auto&&... args) { SetupEntityMeshSystem(args...); });

		world.system<const Transform>("RenderUserEntity")
			.kind(flecs::PreStore)
			.with<NetworkEntityTag>()
			.run([this](auto&&... args) { RenderUserEntitySystem(args...); });
	}
}