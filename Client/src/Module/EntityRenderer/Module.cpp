// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Client/Module/EntityRenderer/Module.h"

#include "Client/Module/Renderer/Module.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Phase.h"

namespace Mcc
{

    EntityRendererModule::EntityRendererModule(flecs::world& world) :
        BaseModule(world),
        mVertexBuffer(GL_ARRAY_BUFFER),
        mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
        mIndexCount(0)
    {}

    void EntityRendererModule::RegisterComponent(flecs::world& /* world */)
    {}

    void EntityRendererModule::RegisterSystem(flecs::world& world)
    {
        world.system("SetupEntityMesh").kind(flecs::OnStart).run([this](auto&&... args) {
            SetupEntityMeshSystem(args...);
        });

        world.system<const Transform>("RenderUserEntity")
            .kind<Phase::OnDraw>()
            .with<NetworkEntityTag>()
            .run([this](auto&&... args) { RenderUserEntitySystem(args...); });
    }

    void EntityRendererModule::RegisterHandler(flecs::world& /* world */)
    {}
}