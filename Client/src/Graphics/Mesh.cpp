//
// Created by Alex on 14/09/2024.
//

#include <Client/Graphics/Mesh.h>
#include <Client/Graphics/Program.h>

#include <algorithm>
#include <iterator>

namespace Mcc
{
	Mesh::Mesh() :
		mVertexBuffer(GL_ARRAY_BUFFER), mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER)
	{}

	void Mesh::SetVertices(std::span<glm::vec3> vertices)
	{
		if (mVertices.empty())
		{
			std::transform(vertices.begin(), vertices.end(), std::back_inserter(mVertices), [](const glm::vec3& v) {
				return PackedVertex { .vertex = v };
			});
		}
		else
		{
			assert(mVertices.size() == vertices.size());
			for (size_t i = 0; i < mVertices.size(); ++i)
			{
				mVertices[i].vertex = vertices[i];
			}
		}
	}

	void Mesh::SetColor(glm::vec3 color)
	{
		for (auto& vertex: mVertices)
		{
			vertex.color = color;
		}
	}

	void Mesh::SetColors(std::span<glm::vec3> colors)
	{
		if (mVertices.empty())
		{
			std::transform(colors.begin(), colors.end(), std::back_inserter(mVertices), [](const glm::vec3& c) {
				return PackedVertex { .color = c };
			});
		}
		else
		{
			assert(mVertices.size() == colors.size());
			for (size_t i = 0; i < mVertices.size(); ++i)
			{
				mVertices[i].color = colors[i];
			}
		}
	}

	void Mesh::SetUvs(std::span<glm::vec2> uvs)
	{
		if (mVertices.empty())
		{
			std::transform(uvs.begin(), uvs.end(), std::back_inserter(mVertices), [](const glm::vec2& uv) {
				return PackedVertex { .uv = uv };
			});
		}
		else
		{
			assert(mVertices.size() == uvs.size());
			for (size_t i = 0; i < mVertices.size(); ++i)
			{
				mVertices[i].uv = uvs[i];
			}
		}
	}

	void Mesh::SetNormals(std::span<glm::vec3> normals)
	{
		if (mVertices.empty())
		{
			std::transform(normals.begin(), normals.end(), std::back_inserter(mVertices), [](const glm::vec3& n) {
				return PackedVertex { .normal = n };
			});
		}
		else
		{
			assert(mVertices.size() == normals.size());
			for (size_t i = 0; i < mVertices.size(); ++i)
			{
				mVertices[i].normal = normals[i];
			}
		}
	}

	void Mesh::SetIndices(std::span<unsigned int> indices)
	{
		std::copy(indices.begin(), indices.end(), std::back_inserter(mIndices));
	}

	void Mesh::Setup(const Program& program)
	{
		mVertexBuffer.Create();
		mVertexBuffer.SetData(std::span(mVertices), GL_STATIC_DRAW);
		program.SetVertexAttribPointer("inVertex", 3, GL_FLOAT, sizeof (PackedVertex), 0);
		program.SetVertexAttribPointer("inColor" , 3, GL_FLOAT, sizeof (PackedVertex), 3 * sizeof (float));

		mIndexBuffer.Create();
		mIndexBuffer.SetData(std::span(mIndices), GL_STATIC_DRAW);
	}

	void Mesh::Draw()
	{
		mIndexBuffer.Bind();
		glCheck(glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr));
	}

}// Mcc