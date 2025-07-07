//
// Created by Alex Clorennec on 06/07/2025.
//

#include <algorithm>

#include "Client/Graphics/PackedVertex.h"

namespace Mcc
{

	bool operator==(const PackedVertex& lhs, const PackedVertex& rhs)
	{
		return lhs.vertex == rhs.vertex && lhs.color == rhs.color && lhs.normal == rhs.normal && lhs.uv == rhs.uv;
	}

	bool operator!=(const PackedVertex& lhs, const PackedVertex& rhs)
	{
		return !(lhs == rhs);
	}

	std::pair<PackedVertexArray, IndexArray> Packed(const PackedVertexArray& vertices)
	{
		PackedVertexArray v{};
		IndexArray        i{};

		for (auto& vertex: vertices)
		{
			if (auto found = std::ranges::find(v, vertex); found != v.end())
			{
				i.push_back(std::distance(v.begin(), found));
			}
			else
			{
				v.push_back(vertex);
				i.push_back(v.size() - 1);
			}
		}

		return { std::move(v), std::move(i) };
	}


}