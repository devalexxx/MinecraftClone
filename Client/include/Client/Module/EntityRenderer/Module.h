// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_ENTITY_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_ENTITY_RENDERER_MODULE_H

#include "Client/Graphics/Buffer.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/VertexArray.h"
#include "Client/Module/Renderer/Module.h"

#include "Common/Module/Base/Module.h"

#include <flecs.h>

namespace Mcc
{

    class EntityRendererModule final : public BaseModule<EntityRendererModule, RendererModule>
    {
      public:
        explicit EntityRendererModule(flecs::world& world);

        void RegisterComponent(flecs::world& world) override;
        void RegisterSystem(flecs::world& world) override;
        void RegisterHandler(flecs::world& world) override;

        void SetupEntityMeshSystem(flecs::iter& it);
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
