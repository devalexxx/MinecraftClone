//
// Created by Alex on 22/09/2024.
//

#include "Client/Graphics/Shader.h"
#include "Client/Graphics/Window/Window.h"
#include "Client/System/Renderer.h"
#include "Client/World/Context.h"
#include "Client/Utils.h"

#include "Client/Module/Camera/Component.h"
#include "Client/Module/Renderer/Module.h"
#include "Common/Module/Entity/Component.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Mcc
{

	Renderer::Renderer(flecs::world& world)
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

		world.system<const Transform>("RenderEntities")
		    .with<NetworkEntityTag>()
			.run([&, this](flecs::iter& it) { RenderEntitiesSystem(it); });
	}

	void Renderer::RenderEntitiesSystem(flecs::iter& it)
	{
		const auto world = it.world();

		mProgram.Use();
		mVertexArray.Bind();

		const auto&& [view, proj] = RendererModule::GetView(world);
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("view"), view);
		mProgram.SetUniformMatrix(mProgram.GetUniformLocation("proj"), proj);

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
}