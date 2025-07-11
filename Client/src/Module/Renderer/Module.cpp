//
// Created by Alex Clorennec on 06/07/2025.
//

#include "Client/Module/Renderer/Module.h"
#include "../../../include/Client/WorldContext.h"
#include "Client/Module/Camera/Component.h"
#include "Client/Module/Renderer/System.h"

#include "Common/Module/Entity/Component.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	RendererModule::RendererModule(flecs::world& world)
	{
		MCC_LOG_DEBUG("Import RendererModule...");
		world.module<RendererModule>();

		world.system("SetupRenderer")
			.kind(flecs::OnStart)
			.run(SetupRendererSystem);

		world.system("PollWindowEvent")
			.kind(flecs::OnLoad)
			.run(PollWindowEventSystem);

		world.system("CleanupFrame")
			.kind(flecs::PreUpdate)
			.run(CleanupFrameSystem);

		world.system("RenderFrame")
			.kind(flecs::OnStore)
			.run(RenderFrameSystem);
	}

	std::pair<glm::mat4, glm::mat4> RendererModule::GetView(const flecs::world& world)
	{
		const auto* ctx = ClientWorldContext::Get(world);

		Transform 	   cTransform {};
		CameraSettings cSettings  {};
		world.query_builder<const Transform, const CameraSettings>().with<ActiveCameraTag>().build().run([&](flecs::iter& it) {
			bool isSet = false;
			while (it.next())
			{
				if (!isSet)
				{
					auto t = it.field<const Transform>(0);
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
						isSet      = true;
					}
				}
			}
		});

		const glm::vec3 up   = glm::normalize(glm::cross(cTransform.rotation * glm::right, cTransform.rotation * glm::forward));
		const glm::mat4 view = glm::lookAt(cTransform.position, cTransform.position + cTransform.rotation * glm::forward, up);
		const glm::mat4 proj = glm::perspective(cSettings.fov, ctx->window.GetAspectRatio(), cSettings.zNear, cSettings.zFar);

		return { view, proj };
	}



}