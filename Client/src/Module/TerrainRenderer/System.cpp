//
// Created by Alex Clorennec on 04/07/2025.
//

#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/Shader.h"
#include "Client/Module/Renderer/Module.h"
#include "Client/Module/TerrainRenderer/Component.h"
#include "Client/Module/TerrainRenderer/Module.h"

#include "Common/Module/Terrain/Component.h"
#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

#include <Hexis/Core/EnumArray.h>

#include <numeric>

#include <glm/ext/matrix_transform.hpp>

namespace Mcc
{

	void TerrainRendererModule::BuildChunkMeshSystem(const flecs::entity entity, ChunkHolder& holder) const
	{
		MCC_LOG_INFO("Building chunk({}) mesh", entity.id());
		const auto world = entity.world();

		PackedVertexArray array{};

		Hx::EnumArray<BlockFace, std::array<glm::vec3, 6>> quadVertices {{
			{ BlockFace::Front,  {{ { -1.f,  1.f,  1.f }, {  1.f, -1.f,  1.f }, {  1.f,  1.f,  1.f }, { -1.f,  1.f,  1.f }, { -1.f, -1.f,  1.f }, {  1.f, -1.f,  1.f } }} },
			{ BlockFace::Back,   {{ {  1.f,  1.f, -1.f }, { -1.f, -1.f, -1.f }, { -1.f,  1.f, -1.f }, {  1.f,  1.f, -1.f }, {  1.f, -1.f, -1.f }, { -1.f, -1.f, -1.f } }} },
			{ BlockFace::Left,   {{ { -1.f, -1.f,  1.f }, { -1.f,  1.f, -1.f }, { -1.f, -1.f, -1.f }, { -1.f, -1.f,  1.f }, { -1.f,  1.f,  1.f }, { -1.f,  1.f, -1.f } }} },
			{ BlockFace::Right,  {{ {  1.f,  1.f,  1.f }, {  1.f, -1.f, -1.f }, {  1.f,  1.f, -1.f }, {  1.f,  1.f,  1.f }, {  1.f, -1.f,  1.f }, {  1.f, -1.f, -1.f } }} },
			{ BlockFace::Bottom, {{ {  1.f, -1.f,  1.f }, { -1.f, -1.f, -1.f }, {  1.f, -1.f, -1.f }, {  1.f, -1.f,  1.f }, { -1.f, -1.f,  1.f }, { -1.f, -1.f, -1.f } }} },
			{ BlockFace::Top,    {{ { -1.f,  1.f,  1.f }, {  1.f,  1.f, -1.f }, { -1.f,  1.f, -1.f }, { -1.f,  1.f,  1.f }, {  1.f,  1.f,  1.f }, {  1.f,  1.f, -1.f } }} }
		}};

		for (size_t x = 0; x < Chunk::Size; ++x)
		{
			for (size_t z = 0; z < Chunk::Size; ++z)
			{
				for (size_t y = 0; y < Chunk::Height; ++y)
				{
					const auto p = glm::uvec3(x, y, z);
					if (auto b = world.entity(holder.chunk->Get(p)); b.is_valid() /* && b.has<BlockTag> */)
					{
						if (b.get<BlockType>() == BlockType::Solid)
						{
							for (auto&& [ face, e ]: holder.chunk->GetNeighbors(p))
							{
								if (auto n = world.entity(e); n == flecs::entity::null() || n.get<BlockType>() != BlockType::Solid)
								{
								    auto color = b.get<BlockColor>().color;
									for (const auto& vertex: quadVertices[face])
									{
										auto	   offset = glm::vec3(p) + ((vertex + 1.f) / 2.f);
										auto       scaled = offset / glm::vec3(Chunk::Size, Chunk::Height, Chunk::Size);
										const auto final  = scaled * 2.f - 1.f;
										array.push_back({final, color, {}, {}});
										// array.push_back({final, { 0, 1, 0 }, {}, {}});
									}
								}
							}
						}
					}
				}
			}
		}

		MCC_LOG_DEBUG("Raw mesh size {}", array.size());
		auto [packed, index] = Index(array);
		MCC_LOG_DEBUG("Packed mesh size {}", packed.size());

		if ([[maybe_unused]] auto mesh = entity.try_get<ChunkMesh>())
		{
			// TODO
		}
		else
		{
			VertexArray vArray{};
			Buffer		vBuffer{GL_ARRAY_BUFFER};
			Buffer		iBuffer{GL_ELEMENT_ARRAY_BUFFER};

			mProgram.Use();
			vArray.Create();
			vArray.Bind();

			vBuffer.Create();
			vBuffer.SetData(std::span(packed), GL_STATIC_DRAW);
			mProgram.SetVertexAttribPointer("inVertex", 3, GL_FLOAT, sizeof (PackedVertex), 0);
			mProgram.SetVertexAttribPointer("inColor" , 3, GL_FLOAT, sizeof (PackedVertex), 3 * sizeof (float));

			iBuffer.Create();
			iBuffer.SetData(std::span(index), GL_STATIC_DRAW);

			entity.set<ChunkMesh>({ std::move(vArray), std::move(vBuffer),std::move(iBuffer), index.size() });
		}
	}

	void TerrainRendererModule::SetupChunkProgramSystem(flecs::iter& it) const
	{
		while (it.next()) {}

		const Shader vertexShader  (GL_VERTEX_SHADER, R"""(
			#version 330

			in vec3 inVertex;
			in vec3 inColor;

			uniform mat4 view;
			uniform mat4 proj;
			uniform mat4 model;

			out vec3 passColor;

			void main() {
				gl_Position = proj * view * model * vec4(inVertex, 1.0);
				passColor = inColor;
			}
		)""");
		const Shader fragmentShader(GL_FRAGMENT_SHADER, R"""(
			#version 330

			in vec3 passColor;

			out vec4 fragment;

			void main() {
				fragment = vec4(passColor, 1.0);
			}
		)""");

		mProgram.Attach(vertexShader);
		mProgram.Attach(fragmentShader);

		mProgram.Link();

		mProgram.Detach(vertexShader);
		mProgram.Detach(fragmentShader);
	}

	void TerrainRendererModule::RenderChunkMeshSystem(flecs::iter& it)
	{
		const auto modelLocation = mProgram.GetUniformLocation("model");

		mProgram.Use();

		const auto&& [view, proj] = RendererModule::GetView(it.world());
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), view);
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("proj"), proj);

		while (it.next())
		{
			auto pos  = it.field<const ChunkPosition>(0);
			auto mesh = it.field<const ChunkMesh>	 (1);

			for (const auto i: it)
			{
				mProgram.SetUniformMatrix(
					modelLocation,
					glm::scale(
						glm::translate(glm::mat4(1.f), glm::vec3(pos[i].position * glm::uvec3(2 * Chunk::Size, 0, 2 * Chunk::Size))),
						glm::vec3(Chunk::Size, Chunk::Height, Chunk::Size)
					)
				);

				mesh[i].vertexArray.Bind();
				mesh[i].indexBuffer.Bind();
				// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				// glDisable(GL_CULL_FACE);
				glCheck(glDrawElements(GL_TRIANGLES, mesh[i].indexCount, GL_UNSIGNED_INT, nullptr));
				// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
	}

}