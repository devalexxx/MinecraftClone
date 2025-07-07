//
// Created by Alex Clorennec on 04/07/2025.
//

#ifndef MCC_CLIENT_GRAPHICS_PACKED_VERTEX_H
#define MCC_CLIENT_GRAPHICS_PACKED_VERTEX_H

#include <glm/glm.hpp>

#include <vector>

namespace Mcc
{

	struct PackedVertex
	{
		glm::vec3 vertex;
		glm::vec3 color;
		glm::vec2 uv;
		glm::vec3 normal;
	};

	bool operator==(const PackedVertex& lhs, const PackedVertex& rhs);
	bool operator!=(const PackedVertex& lhs, const PackedVertex& rhs);

	using PackedVertexArray = std::vector<PackedVertex>;
	using IndexArray		= std::vector<unsigned int>;

	std::pair<PackedVertexArray, IndexArray> Packed(const PackedVertexArray& vertices);

}

#endif
