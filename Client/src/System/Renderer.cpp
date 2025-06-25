//
// Created by Alex on 22/09/2024.
//

#include "Client/Graphics/Shader.h"
#include "Client/Graphics/Window/Window.h"
#include "Client/System/Renderer.h"
#include "Client/World/Context.h"
#include "Client/Utils.h"

#include "Common/Network/Packet.h"

#include <flecs.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Mcc
{

	Renderer::Renderer(flecs::world& world) : mProjection(glm::perspective(glm::radians(75.f), 1024.f / 720.f, .0f, 100.f))
	{
		world.module<Renderer>();

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

		world.system<const Transform>()
			.run([&, this](flecs::iter& it) {
				static std::array<glm::mat4, 3> models = {{
					glm::translate(glm::mat4(1.f), { 0, 0, 0 }),
					glm::translate(glm::mat4(1.f), { 0, 1, 0 }),
					glm::translate(glm::mat4(1.f), { 0, 2, 0 }),
				}};

				const auto ctx = static_cast<ClientWorldContext*>(world.get_ctx());

				auto id = ctx->networkToLocal.find(ctx->playerInfo.id);
				if (id != ctx->networkToLocal.cend() && world.exists(id->second))
				{
					flecs::entity entity = world.entity(id->second);
					auto* t = entity.get<Transform>();

					glm::vec3 up   = glm::cross(glm::right(t->rotation), glm::forward(t->rotation));
					glm::mat4 view = glm::lookAt(t->position, t->position + glm::forward(t->rotation), up);

					glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
					glCheck(glClearColor(1., 1., 1., 1.));

					mProgram.Use();
					mVertexArray.Bind();

					mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), view);
					mProgram.SetUniformMatrix(mProgram.GetUniformLocation("projection"), mProjection);

					for (auto& model: models)
					{
						mProgram.SetUniformMatrix(mProgram.GetUniformLocation("model"), model);
						mCube.Draw();
					}

					while (it.next())
					{
						auto tr = it.field<const Transform>(0);

						for (auto i: it)
						{
							mProgram.SetUniformMatrix(mProgram.GetUniformLocation("model"), glm::translate(glm::mat4(1.f), tr[i].position - glm::vec3(0, 2, 0)));
							mCube.Draw();
						}
					}
				}
				else
				{
					while (it.next());
				}

				ctx->window.SwapBuffer();
				Window::PollEvents();
			});
	}
}