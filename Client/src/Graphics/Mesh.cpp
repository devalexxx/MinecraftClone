//
// Created by Alex on 14/09/2024.
//

#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/Program.h"

#include <algorithm>
#include <iterator>

namespace Mcc
{
	Mesh::Mesh() :
		mVertexBuffer(GL_ARRAY_BUFFER), mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER)
	{}

	void Mesh::SetPackedVertex(std::span<PackedVertex> vertices)
	{
		if (mVertices.size() < vertices.size())
			mVertices.resize(vertices.size());

		std::copy(vertices.begin(), vertices.end(), mVertices.begin());
	}

	void Mesh::SetVertices(std::span<glm::vec3> vertices)
	{
		if (mVertices.size() < vertices.size())
			mVertices.resize(vertices.size());

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			mVertices[i].vertex = vertices[i];
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
		if (mVertices.size() < colors.size())
			mVertices.resize(colors.size());

		for (size_t i = 0; i < colors.size(); ++i)
		{
			mVertices[i].color = colors[i];
		}
	}

	void Mesh::SetUvs(std::span<glm::vec2> uvs)
	{
		if (mVertices.size() < uvs.size())
			mVertices.resize(uvs.size());

		for (size_t i = 0; i < uvs.size(); ++i)
		{
			mVertices[i].uv = uvs[i];
		}
	}

	void Mesh::SetNormals(std::span<glm::vec3> normals)
	{
		if (mVertices.size() < normals.size())
			mVertices.resize(normals.size());

		for (size_t i = 0; i < normals.size(); ++i)
		{
			mVertices[i].normal = normals[i];
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