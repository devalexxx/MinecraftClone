// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_ENTITY_RENDERER_MODULE_H
#define MCC_CLIENT_MODULE_ENTITY_RENDERER_MODULE_H

#include "Client/Graphics/Buffer.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/VertexArray.h"

#include <flecs.h>

namespace Mcc
{

    class EntityRendererModule
    {
      public:
        explicit EntityRendererModule(const flecs::world& world);

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
