//
// Created by Alex on 01/07/2025.
//

#ifndef MCC_CLIENT_MODULE_TERRAIN_RENDERER_COMPONENT_H
#define MCC_CLIENT_MODULE_TERRAIN_RENDERER_COMPONENT_H

#include "Client/Graphics/Buffer.h"
#include "Client/Graphics/VertexArray.h"

namespace Mcc
{

	struct ChunkMesh
	{
		VertexArray	vertexArray;
		Buffer		vertexBuffer { GL_ARRAY_BUFFER };
		Buffer 		indexBuffer  { GL_ELEMENT_ARRAY_BUFFER };
		size_t		indexCount;
	};

}

#endif
