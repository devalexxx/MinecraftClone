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

		world.system<const Transform>()
		    .with<NetworkEntityTag>()
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
					Transform 	   cTransform {};
					CameraSettings cSettings  {};
					mCameraQuery.run([&](flecs::iter& it) {
						it.next();

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
						}

						while (it.next()) {}
					});

					glm::vec3 up   = glm::normalize(glm::cross(cTransform.rotation * glm::right, cTransform.rotation * glm::forward));
					glm::mat4 view = glm::lookAt(cTransform.position, cTransform.position + cTransform.rotation * glm::forward, up);
					glm::mat4 proj = glm::perspective(cSettings.fov, ctx->window.GetAspectRatio(), cSettings.zNear, cSettings.zFar);

					glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
					glCheck(glClearColor(1., 1., 1., 1.));

					mProgram.Use();
					mVertexArray.Bind();

					mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), view);
					mProgram.SetUniformMatrix(mProgram.GetUniformLocation("projection"), proj);

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