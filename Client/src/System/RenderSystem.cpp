//
// Created by Alex on 22/09/2024.
//

#include <Client/System/RenderSystem.h>
#include <Client/Utils.h>
#include <Client/Graphics/Shader.h>
#include <Client/Graphics/Window/Window.h>

#include <MinecraftLib/Network/Packet.h>

#include <flecs.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Mcc
{

	RenderSystem::RenderSystem(flecs::world& world, const PlayerInfo& info, Window& window) :
		mWorld(world),
		mInfo(info),
		mWindow(window),
		mProjection(glm::perspective(glm::radians(75.f), 1024.f / 720.f, .0f, 100.f))
	{
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

//		mWorld.system().run(this->SetupRender);

		mWorld.system()
			.run([this](flecs::iter&) {
				static std::array<glm::mat4, 3> models = {{
					glm::translate(glm::mat4(1.f), { 0, 0, 0 }),
					glm::translate(glm::mat4(1.f), { 0, 1, 0 }),
					glm::translate(glm::mat4(1.f), { 0, 2, 0 }),
				}};

				if (mWorld.exists(mInfo.entity))
				{
					flecs::entity entity = mWorld.entity(mInfo.entity);
					auto* p = entity.get<Position>();
					auto* d = entity.get<Forward>();
					auto* r = entity.get<Right>();

					glm::vec3 up = glm::cross(r->vec, d->vec);

					glm::mat4 view = glm::lookAt(p->vec, p->vec + d->vec, up);

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
				}

				mWindow.SwapBuffer();
				Window::PollEvents();
			});
	}

	void RenderSystem::SetupRender(flecs::iter&)
	{
	}
	void RenderSystem::SetupCamera(flecs::iter&)
	{
	}
	void RenderSystem::RenderWorld(flecs::iter&)
	{
	}
	void RenderSystem::UpdateWindow(flecs::iter&)
	{
	}

}