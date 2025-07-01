//
// Created by Alex on 22/09/2024.
//

#include "Client/Graphics/Shader.h"
#include "Client/Graphics/Window/Window.h"
#include "Client/System/Renderer.h"
#include "Client/World/Context.h"
#include "Client/Utils.h"

#include "Client/Module/Camera/Component.h"
#include "Common/Module/Entity/Component.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Mcc
{

	Renderer::Renderer(flecs::world& world)
	{
		world.module<Renderer>();

		mCameraQuery = world.query_builder<const Transform, const CameraSettings>().with<ActiveCameraTag>().build();

		mCube.SetVertices(vertices);
		mCube.SetColor({ .9f, .9f, .0f });
		mCube.SetIndices(elements);

		{
			Shader vertexShader(GL_VERTEX_SHADER, vertexCode);
			Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentCode);

			mProgram.Attach(vertexShader);
			mProgram.Attach(fragmentShader);

			mProgram.Link();

			mProgram.Detach(vertexShader);
			mProgram.Detach(fragmentShader);
		}

		mProgram.Use();
		mVertexArray.Create();
		mVertexArray.Bind();

		mCube.Setup(mProgram);

		glEnable(GL_DEPTH);
		glDepthFunc(GL_LEFT);
		glEnable(GL_CULL_FACE_MODE);

		world.system("PreRender")
			.run([](flecs::iter& it) {
				while (it.next());
				glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
				glCheck(glClearColor(1., 1., 1., 1.));
			});

		world.system<const Transform>("RenderEntities")
		    .with<NetworkEntityTag>()
			.run([&, this](flecs::iter& it) { RenderEntitiesSystem(it); });

		world.system<const ChunkPosition, const ChunkData>("RenderTerrain")
			.with<ChunkTag>()
			.run([&, this](flecs::iter& it) { RenderTerrainSystem(it); });

		world.system("PostRender")
			.run([](flecs::iter& it) {
				while (it.next());
				const auto ctx = static_cast<ClientWorldContext*>(it.world().get_ctx());
				ctx->window.SwapBuffer();
				Window::PollEvents();
			});
	}

	std::pair<glm::mat4, glm::mat4> Renderer::GetCamera(const ClientWorldContext* ctx) const
	{
		Transform 	   cTransform {};
		CameraSettings cSettings  {};
		mCameraQuery.run([&](flecs::iter& it) {
			bool isSet = false;
			while (it.next())
			{
				if (!isSet)
				{
					auto t = it.field<const Transform>	   (0);
					auto s = it.field<const CameraSettings>(1);

					if (it.count() > 1)
						MCC_LOG_WARN("More than one camera active");

					if (it.count() <= 0)
					{
						MCC_LOG_ERROR("No active camera");
					}
					else
					{
						cTransform = t[0];
						cSettings  = s[0];
						isSet = true;
					}
				}
			}
		});

		glm::vec3 up   = glm::normalize(glm::cross(cTransform.rotation * glm::right, cTransform.rotation * glm::forward));
		glm::mat4 view = glm::lookAt(cTransform.position, cTransform.position + cTransform.rotation * glm::forward, up);
		glm::mat4 proj = glm::perspective(cSettings.fov, ctx->window.GetAspectRatio(), cSettings.zNear, cSettings.zFar);

		return { view, proj };
	}

	void Renderer::RenderEntitiesSystem(flecs::iter& it)
	{
		const auto world = it.world();
		const auto ctx   = static_cast<ClientWorldContext*>(world.get_ctx());

		auto [view, proj] = GetCamera(ctx);

		mProgram.Use();
		mVertexArray.Bind();

		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), 		 view);
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("projection"), proj);

		while (it.next())
		{
			auto tr = it.field<const Transform>(0);

			for (auto i: it)
			{
				mProgram.SetUniformMatrix(
					mProgram.GetUniformLocation("model"),
					glm::translate(glm::mat4(1.f), tr[i].position) * glm::toMat4(tr[i].rotation) * glm::scale(glm::mat4(1.f), tr[i].scale)
				);
				mCube.Draw();
			}
		}
	}

	void Renderer::RenderTerrainSystem(flecs::iter& it)
	{
		const auto world = it.world();
		const auto ctx   = static_cast<ClientWorldContext*>(world.get_ctx());

		auto [view, proj] = GetCamera(ctx);

		mProgram.Use();
		mVertexArray.Bind();

		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), 		 view);
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("projection"), proj);

		while (it.next())
		{
			auto p = it.field<const ChunkPosition>(0);
			auto d = it.field<const ChunkData>	  (1);

			for (auto i: it)
			{
				std::vector<flecs::entity_t> toRender;
				for (auto& e: d[0].data->GetPalette())
				{
					world.entity(e).get([&](const BlockMeta& meta) {
						if (meta.id != "mcc:block:air")
							toRender.push_back(e);
					});
				}

				for (int x = 0; x < Chunk::Size; ++x)
				{
					for (int z = 0; z < Chunk::Size; ++z)
					{
						for (int y = 0; y < Chunk::Height; ++y)
						{
							auto block = d[i].data->Get({x, y, z});
							world.entity(block).get([&](const BlockMeta& meta) {
								if (meta.id != "mcc:block:air")
								{
									mProgram.SetUniformMatrix(
										mProgram.GetUniformLocation("model"),
										glm::translate(glm::mat4(1.f), glm::vec3(p[i].position * glm::uvec3(Chunk::Size, 0, Chunk::Size) + glm::uvec3(x, y, z )))
									);
									mCube.Draw();
								}
						    });
						}
					}
				}
			}
		}
	}
}