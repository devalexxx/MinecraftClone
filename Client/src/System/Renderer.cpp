//
// Created by Alex on 22/09/2024.
//

#include "Client/Graphics/Shader.h"
#include "Client/Graphics/Window/Window.h"
#include "Client/System/Renderer.h"
#include "Client/World/Context.h"
#include "Client/Utils.h"
#include "Client/Module/Camera/Tag.h"

#include "Common/Module/WorldEntity/Tag.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Mcc
{

	Renderer::Renderer(flecs::world& world) : mProjection(glm::perspective(glm::radians(75.f), 1024.f / 720.f, .0f, 100.f))
	{
		world.module<Renderer>();

		mCameraQuery = world.query_builder<const Transform, const Transform>().with<ActiveCameraTag>().term_at(1).parent().cascade().build();

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
		    .with<WorldEntityTag>()
			.run([&, this](flecs::iter& it) {
				static std::array<glm::mat4, 3> models = {{
					glm::translate(glm::mat4(1.f), { 0, 0, 0 }),
					glm::translate(glm::mat4(1.f), { 0, 2, 0 }),
					glm::translate(glm::mat4(1.f), { 0, 4, 0 }),
				}};

				const auto ctx = static_cast<ClientWorldContext*>(world.get_ctx());

				auto id = ctx->networkToLocal.find(ctx->playerInfo.id);
				if (id != ctx->networkToLocal.cend() && world.exists(id->second))
				{
					Transform cTransform {};
					mCameraQuery.each([&cTransform](const Transform& transform, const Transform& pTransform) {
						cTransform.position = pTransform.position + transform.position;
						cTransform.rotation = glm::normalize(pTransform.rotation * transform.rotation);
						cTransform.scale 	= transform.scale;
					});

					glm::vec3 up   = glm::normalize(glm::cross(cTransform.rotation * glm::right, cTransform.rotation * glm::forward));
					glm::mat4 view = glm::lookAt(cTransform.position, cTransform.position + cTransform.rotation * glm::forward, up);

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
							mProgram.SetUniformMatrix(
								mProgram.GetUniformLocation("model"),
								glm::translate(glm::mat4(1.f), tr[i].position) * glm::toMat4(tr[i].rotation) * glm::scale(glm::mat4(1.f), tr[i].scale)
							);
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