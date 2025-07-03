//
// Created by Alex on 28/06/2025.
//

#include "Client/Module/Player/Module.h"
#include "Client/Module/Player/Component.h"
#include "Client/Module/Player/System.h"
#include "Client/Module/Camera/Module.h"
#include "Client/Module/Camera/Component.h"
#include "Client/Module/EntityReplication/Module.h"
#include "Client/Module/EntityReplication/Component.h"
#include "Client/World/Context.h"

#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	PlayerModule::PlayerModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<EntityReplicationModule>(), "PlayerInputModule require EntityPredictionModule, you must import it before.");
		MCC_ASSERT	 (world.has<CameraModule>()    		  , "PlayerInputModule require CameraModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerInputModule...");
		world.module<PlayerModule>();

		world.component<PlayerEntityTag>();

		world.component<CurrentPlayerInput>();

		world.prefab<PlayerEntityPrefab>()
			.is_a<UserEntityPrefab>()
			.add<PlayerEntityTag>()
			.add<InterpolationExcludedTag>()
			.set_auto_override<CurrentPlayerInput>({});

		world.system<CurrentPlayerInput, UserInputQueue>().with<PlayerEntityTag>().each(ApplyAndSendPlayerInput);

		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnEntitiesCreated>([&world](const auto& event) { OnEntitiesCreatedHandler(world, event); });
		ctx->networkManager.Subscribe<OnEntitiesUpdated>([&world](const auto& event) { OnEntitiesUpdatedHandler(world, event); });

		ctx->window.Subscribe<KeyEvent>		 ([&world](const auto& event) { OnKeyEventHandler	   (world, event); });
		ctx->window.Subscribe<CursorPosEvent>([&world](const auto& event) { OnCursorPosEventHandler(world, event); });
	}

	void PlayerModule::OnEntitiesCreatedHandler(flecs::world& world, const OnEntitiesCreated& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		const auto state = std::find_if(event.states.begin(), event.states.end(), [&](const EntityState& s) { return s.id == ctx->playerInfo.id; });
		if (state != event.states.cend())
		{
			auto it = ctx->networkToLocal.find(ctx->playerInfo.id);
			if (it == ctx->networkToLocal.cend())
			{
				MCC_LOG_WARN("No local entity associated to the player network id, make sure to import EntityReplicationModule before PlayerInputModule");
				return;
			}
			auto entity = world.entity(it->second).is_a<PlayerEntityPrefab>();
			world.entity().is_a<CameraFollowPrefab>()
			    .set<CameraSettings>	  ({ glm::radians(75.f), 0.f, 100.f })
			    .set<CameraFollowSettings>({ { 0, 2, 0 } })
				.add<CameraFollowRelation>(entity)
			    .add<ActiveCameraTag>();
		}
	}

	void PlayerModule::OnEntitiesUpdatedHandler(flecs::world& world, const OnEntitiesUpdated& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		const auto state = std::find_if(event.states.begin(), event.states.end(), [&](const EntityState& s) { return s.id == ctx->playerInfo.id; });
		if (state != event.states.cend())
		{
			const auto lipIt = state->extra.find("last-input-processed");
			if (lipIt != state->extra.cend())
			{
				const auto id = std::stoul(lipIt->second);
				auto idIt 	  = ctx->networkToLocal.find(state->id);
				if (idIt == ctx->networkToLocal.cend())
				{
					MCC_LOG_WARN("The player network id {} isn't associated to a local entity", state->id);
					return;
				}
				auto entity = world.entity(idIt->second);
				auto& queue = entity.get_mut<UserInputQueue>();

				// Drop inputs already processed by the server
				for (; !queue.data.empty(); queue.data.pop_front())
				{
					auto& input = queue.data.front();
					if (input.meta.id == id)
					{
						queue.data.pop_front();
						break;
					}
				}

				MCC_ASSERT(queue->data.empty() || (!queue->data.empty() && queue->data.front().meta.id - id == 1), "The difference between front Player in queue and last Player processes by server should be equal to 1");

				// Empty snapshot queue
				auto& q = entity.get_mut<SnapshotQueue>();
				while (!q.data.empty()) { q.data.pop_back(); }

				// Set last received transform & reapply all input unprocessed by the server
				entity.set(state->transform);
				for (auto& input: queue.data)
				{
					const float speed = 5.f;
					entity.get([&input, speed](Transform& transform) {
						Helper::ApplyXAxis	 (input, transform);
						Helper::ApplyMovement(input, transform, speed, input.meta.dt);
					});
				}
			}
		}
	}

	void PlayerModule::OnKeyEventHandler(flecs::world& world, const KeyEvent& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());
		const auto  it  = ctx->networkToLocal.find(ctx->playerInfo.id);
		if (it == ctx->networkToLocal.cend())
		{
			MCC_LOG_WARN("The player network id {} isn't associated to a local entity", ctx->playerInfo.id);
			return;
		}
		auto entity = world.entity(it->second);
		auto input  = entity.get_ref<CurrentPlayerInput>()->input;

		if (event.action == GLFW_PRESS || event.action == GLFW_RELEASE)
		{
			switch (event.key)
			{
				case GLFW_KEY_W:
					input.movement.forward = event.action == GLFW_PRESS;
					break;
				case GLFW_KEY_S:
					input.movement.backward = event.action == GLFW_PRESS;
					break;
				case GLFW_KEY_A:
					input.movement.left = event.action == GLFW_PRESS;
					break;
				case GLFW_KEY_D:
					input.movement.right = event.action == GLFW_PRESS;
					break;
				case GLFW_KEY_ESCAPE:
					event.window.SetShouldClose();
				default:
					break;
			}
		}
	}

	void PlayerModule::OnCursorPosEventHandler(flecs::world& world, const CursorPosEvent& event)
	{
		auto sensitivity = 0.001f;
		auto [w, h] = event.window.GetWindowSize();

		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());
		const auto  it 	= ctx->networkToLocal.find(ctx->playerInfo.id);
		if (it == ctx->networkToLocal.cend())
		{
			MCC_LOG_WARN("The player network id {} isn't associated to a local entity", ctx->playerInfo.id);
			return;
		}
		auto  entity = world.entity(it->second);
		auto& input  = entity.get_ref<CurrentPlayerInput>()->input;

		if (event.x >= 0 && event.x <= w && event.y >= 0 && event.y <= h && event.window.IsFocused())
		{
			event.window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			input.axis.x += sensitivity * ((static_cast<float>(w) / 2.f) - static_cast<float>(event.x));
			input.axis.y += sensitivity * ((static_cast<float>(h) / 2.f) - static_cast<float>(event.y));

			event.window.SetCursorPosition(w / 2, h / 2);
		}
	}

}