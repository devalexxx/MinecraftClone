//
// Created by Alex Clorennec on 04/07/2025.
//

#include "Client/Graphics/Mesh.h"
#include "Client/Graphics/Program.h"
#include "Client/Graphics/Shader.h"
#include "Client/Module/Renderer/Module.h"
#include "Client/Module/TerrainRenderer/Component.h"
#include "Client/Module/TerrainRenderer/Module.h"
#include "Client/Module/TerrainReplication/Component.h"

#include "Common/Module/Entity/Component.h"
#include "Common/WorldContext.h"
#include "Common/Module/Terrain/Component.h"

#include <Hexis/Core/EnumArray.h>

#include <numeric>

#include <glm/ext/matrix_transform.hpp>

namespace Mcc
{

	void TerrainRendererModule::BuildChunkMeshSystem(const flecs::entity entity, const ChunkHolder& holder) const
	{
		const auto  world = entity.world();
        const auto* ctx   = WorldContext<>::Get(world);
	    auto chunk = holder.chunk;

	    auto meshFuture = ctx->threadPool.ExecuteTask([world, chunk] {
	        static Hx::EnumArray<BlockFace, std::array<glm::vec3, 6>> quadVertices {{
                { BlockFace::Front,  {{ { -1.f,  1.f,  1.f }, {  1.f, -1.f,  1.f }, {  1.f,  1.f,  1.f }, { -1.f,  1.f,  1.f }, { -1.f, -1.f,  1.f }, {  1.f, -1.f,  1.f } }} },
                { BlockFace::Back,   {{ {  1.f,  1.f, -1.f }, { -1.f, -1.f, -1.f }, { -1.f,  1.f, -1.f }, {  1.f,  1.f, -1.f }, {  1.f, -1.f, -1.f }, { -1.f, -1.f, -1.f } }} },
                { BlockFace::Left,   {{ { -1.f, -1.f,  1.f }, { -1.f,  1.f, -1.f }, { -1.f, -1.f, -1.f }, { -1.f, -1.f,  1.f }, { -1.f,  1.f,  1.f }, { -1.f,  1.f, -1.f } }} },
                { BlockFace::Right,  {{ {  1.f,  1.f,  1.f }, {  1.f, -1.f, -1.f }, {  1.f,  1.f, -1.f }, {  1.f,  1.f,  1.f }, {  1.f, -1.f,  1.f }, {  1.f, -1.f, -1.f } }} },
                { BlockFace::Bottom, {{ {  1.f, -1.f,  1.f }, { -1.f, -1.f, -1.f }, {  1.f, -1.f, -1.f }, {  1.f, -1.f,  1.f }, { -1.f, -1.f,  1.f }, { -1.f, -1.f, -1.f } }} },
                { BlockFace::Top,    {{ { -1.f,  1.f,  1.f }, {  1.f,  1.f, -1.f }, { -1.f,  1.f, -1.f }, { -1.f,  1.f,  1.f }, {  1.f,  1.f,  1.f }, {  1.f,  1.f, -1.f } }} }
             }};

            static Hx::EnumArray<BlockFace, glm::vec3> quadNormals {{
                { BlockFace::Front,  glm::forward },
                { BlockFace::Back,   glm::back    },
                { BlockFace::Left,   glm::left    },
                { BlockFace::Right,  glm::right   },
                { BlockFace::Bottom, glm::down    },
                { BlockFace::Top,    glm::up      }
            }};

	        PackedVertexArray array{};

            std::unordered_map<flecs::entity_t, std::tuple<BlockType, BlockColor>> cache{};
            for (auto& e: chunk->GetPalette())
            {
                world.entity(e).get([e, &cache](const BlockType& type, const BlockColor& color) {
                    cache.emplace(e, std::make_tuple(type, color));
                });
            }

            for (size_t x = 0; x < Chunk::Size; ++x)
            {
                for (size_t z = 0; z < Chunk::Size; ++z)
                {
                    for (size_t y = 0; y < Chunk::Height; ++y)
                    {
                        const auto p = glm::uvec3(x, y, z);
                        if (auto b = cache.find(chunk->Get(p)); b != cache.end())
                        {
                            if (auto [type, color] = b->second; type == BlockType::Solid)
                            {
                                for (auto&& [ face, e ]: chunk->GetNeighbors(p))
                                {
                                    if (auto n = cache.find(e); n == cache.end() || std::get<BlockType>(n->second) != BlockType::Solid)
                                    {
                                        for (const auto& vertex: quadVertices[face])
                                        {
                                            auto	   offset = glm::vec3(p) + ((vertex + 1.f) / 2.f);
                                            auto       scaled = offset / glm::vec3(Chunk::Size, Chunk::Height, Chunk::Size);
                                            const auto final  = scaled * 2.f - 1.f;
                                            array.push_back({final, color.color, {}, quadNormals[face]});
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

	        return Index(array);
	    });

        entity.set<MeshHolder>({ (std::move(meshFuture)) });
	    entity.remove<ChunkDirtyTag>();
	}

    void TerrainRendererModule::SetupChunkRenderingMeshSystem(const flecs::entity entity, MeshHolder& holder) const
	{
	    if (holder.pendingMesh.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
	    {
	        if ([[maybe_unused]] auto mesh = entity.try_get<ChunkMesh>())
	        {
	            // TODO
	        }
	        else
	        {
	            auto [vertex, index] = holder.pendingMesh.get();
	            VertexArray vArray{};
	            Buffer		vBuffer{GL_ARRAY_BUFFER};
	            Buffer		iBuffer{GL_ELEMENT_ARRAY_BUFFER};

	            mProgram.Use();
	            vArray.Create();
	            vArray.Bind();

	            vBuffer.Create();
	            vBuffer.SetData(std::span(vertex), GL_STATIC_DRAW);
	            mProgram.SetVertexAttribPointer("inVertex", 3, GL_FLOAT, sizeof (PackedVertex), 0);
	            mProgram.SetVertexAttribPointer("inColor" , 3, GL_FLOAT, sizeof (PackedVertex), 3 * sizeof (float));
	            mProgram.SetVertexAttribPointer("inNormal", 3, GL_FLOAT, sizeof (PackedVertex), 8 * sizeof (float));

	            iBuffer.Create();
	            iBuffer.SetData(std::span(index), GL_STATIC_DRAW);

	            entity.set<ChunkMesh>({ std::move(vArray), std::move(vBuffer),std::move(iBuffer), index.size() });
	            entity.remove<MeshHolder>();
	        }
	    }
	}

	void TerrainRendererModule::SetupChunkProgramSystem(flecs::iter& it) const
	{
		while (it.next()) {}

		const Shader vertexShader  (GL_VERTEX_SHADER, R"""(
			#version 330

			in vec3 inVertex;
			in vec3 inColor;
            in vec3 inNormal;

			uniform mat4 view;
			uniform mat4 proj;
			uniform mat4 model;

			out vec3 passColor;
            out vec3 passPos;
            out vec3 passNormal;

			void main() {
				gl_Position = proj * view * model * vec4(inVertex, 1.0);
				passColor  = inColor;
                passPos    = vec3(model * vec4(inVertex, 1.0));
                passNormal = inNormal;
			}
		)""");
		const Shader fragmentShader(GL_FRAGMENT_SHADER, R"""(
			#version 330

			in vec3 passColor;
            in vec3 passPos;
            in vec3 passNormal;

            uniform mat3 invModel;
            uniform vec3 viewPos;

			out vec4 fragment;

            vec3 lightPos   = vec3(0, 100, 0);
            vec3 lightColor = vec3(1, 1, 1);

            float ambientStrength  = 0.1;
            float specularStrength = 1;

			void main() {
                vec3 normal = invModel * passNormal;
                vec3 norm = normalize(normal);
                vec3 lightDir = normalize(lightPos - passPos);

                // Ambient lighting
                vec3 ambient = ambientStrength * lightColor;

                // Diffuse lighting
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;

                // Specular lighting
                vec3 viewDir = normalize(viewPos - passPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * lightColor;

				fragment = vec4((ambient + diffuse + specular) * passColor, 1.0);
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
		const auto modelLocation    = mProgram.GetUniformLocation("model");
		const auto invModelLocation = mProgram.GetUniformLocation("invModel");

		mProgram.Use();

		const auto&& [p, view, proj] = RendererModule::GetView(it.world());
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), view);
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("proj"), proj);

	    mProgram.SetUniformVector(mProgram.GetUniformLocation("viewPos"), p);

		while (it.next())
		{
			auto pos  = it.field<const ChunkPosition>(0);
			auto mesh = it.field<const ChunkMesh>	 (1);

			for (const auto i: it)
			{
			    glm::mat4 model = glm::scale(
                    glm::translate(glm::mat4(1.f), glm::vec3(pos[i].position * glm::ivec3(2 * Chunk::Size, 0, 2 * Chunk::Size))),
                    glm::vec3(Chunk::Size, Chunk::Height, Chunk::Size)
                );

                mProgram.SetUniformMatrix(invModelLocation, glm::transpose(glm::inverse(glm::mat3(model))));
				mProgram.SetUniformMatrix(modelLocation   , model);

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