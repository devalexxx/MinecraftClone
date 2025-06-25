//
// Created by Alex on 23/06/2025.
//

#include "Client/Module/PlayerInput/Module.h"
#include "Client/Module/PlayerInput/Component.h"
#include "Client/Module/PlayerInput/System.h"
#include "Client/Module/PlayerInput/Prefab.h"
#include "Client/Module/PlayerInput/Tag.h"
#include "Client/World/Context.h"

#include "Common/Utils/Assert.h"
#include "Common/Utils/Logging.h"
#include "Common/Module/PlayerEntity/Component.h"
#include "Common/Module/PlayerEntity/Prefab.h"

namespace Mcc
{

	PlayerInputModule::PlayerInputModule(flecs::world& world)
	{
		MCC_ASSERT	 (world.has<PlayerEntityModule>(), "PlayerInputModule require PlayerEntityModule, you must import it before.");
		MCC_LOG_DEBUG("Import PlayerInputModule...");
		world.module<PlayerInputModule>();

		world.component<CurrentPlayerInput>();
		world.component<SelfPlayerEntityTag>();

		world.prefab<SelfPlayerEntityPrefab>()
		    .is_a<PlayerEntityPrefab>()
			.add<SelfPlayerEntityTag>()
			.set_auto_override<CurrentPlayerInput>({});

		world.system<CurrentPlayerInput, PlayerInputQueue>().with<SelfPlayerEntityTag>().each(ApplyAndSendPlayerInput);

		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());

		ctx->networkManager.Subscribe<OnEntitiesCreated>([&world](const auto& event) { OnEntitiesCreatedHandler(world, event); });
		ctx->networkManager.Subscribe<OnEntitiesUpdated>([&world](const auto& event) { OnEntitiesUpdatedHandler(world, event); });

		ctx->window.Subscribe<KeyEvent>		 ([&](const auto& event) { OnKeyEventHandler	  (world, event); });
		ctx->window.Subscribe<CursorPosEvent>([&](const auto& event) { OnCursorPosEventHandler(world, event); });
	}

	void PlayerInputModule::OnEntitiesCreatedHandler(flecs::world& world, const OnEntitiesCreated& event)
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
			world.entity(it->second).is_a<SelfPlayerEntityPrefab>();
		}
	}

	void PlayerInputModule::OnEntitiesUpdatedHandler(flecs::world& world, const OnEntitiesUpdated& event)
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
				auto queue 	= entity.get_mut<PlayerInputQueue>();

				// Drop inputs already processed by the server
				for (; !queue->data.empty(); queue->data.pop_front())
				{
					auto& input = queue->data.front();
					if (input.meta.id == id)
					{
						queue->data.pop_front();
						break;
					}
				}

				MCC_ASSERT(queue->data.empty() || (!queue->data.empty() && queue->data.front().meta.id - id == 1), "The difference between front PlayerInput in queue and last PlayerInput processes by server should be equal to 1");

				// Reapply all input unprocessed by the server
				for (auto& input: queue->data)
				{
					const float speed = 5.f;
					entity.get([&input, speed](Transform& transform) {
						input.Apply(transform);
						input.Apply(transform, input.meta.dt, speed);
					});
				}
			}
		}
	}

	void PlayerInputModule::OnKeyEventHandler(flecs::world& world, const KeyEvent& event)
	{
		const auto* ctx = static_cast<ClientWorldContext*>(world.get_ctx());
		const auto  it  = ctx->networkToLocal.find(ctx->playerInfo.id);
		if (it == ctx->networkToLocal.cend())
		{
			MCC_LOG_WARN("The player network id {} isn't associated to a local entity", ctx->playerInfo.id);
			return;
		}
		auto  entity = world.entity(it->second);
		auto& input  = entity.get_ref<CurrentPlayerInput>()->input;

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

	void PlayerInputModule::OnCursorPosEventHandler(flecs::world& world, const CursorPosEvent& event)
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