//
// Created by Alex on 14/09/2024.
//

#ifndef MINECRAFT_MESH_H
#define MINECRAFT_MESH_H

#include <Client/Graphics/Buffer.h>

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

	class Program;

	class Mesh
	{
		public:
			Mesh();

			void SetVertices(std::span<glm::vec3> vertices);
			void SetColor(glm::vec3 color);
			void SetColors(std::span<glm::vec3> colors);
			void SetUvs(std::span<glm::vec2> uvs);
			void SetNormals(std::span<glm::vec3> normals);
			void SetIndices(std::span<unsigned int> indices);

			void Setup(const Program& program);
			void Draw();

		private:
			std::vector<PackedVertex> mVertices;
			std::vector<unsigned int> mIndices;

			Buffer mVertexBuffer;
			Buffer mIndexBuffer;
	};

}

#endif
