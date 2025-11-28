// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_TERRAIN_RENDERER_COMPONENT_H
#define MCC_CLIENT_MODULE_TERRAIN_RENDERER_COMPONENT_H

#include "Client/Graphics/Buffer.h"
#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/VertexArray.h"

#include "Hexis/Core/Task.h"

namespace Mcc
{

    struct ShouldBuildMeshTag
    {};
    struct CouldRenderChunkTag
    {};
    struct ShouldRenderChunkTag
    {};

    struct ChunkMesh
    {
        VertexArray vertexArray;
        Buffer      vertexBuffer { GL_ARRAY_BUFFER };
        Buffer      indexBuffer { GL_ELEMENT_ARRAY_BUFFER };
        size_t      indexCount;
    };


    struct MeshHolder
    {
        Hx::UniqueTaskRef<Mesh> pendingMesh;
    };

}

#endif
