//
// Created by Alex on 02/09/2024.
//

#include <Server/ServerApplication.h>

#include <MinecraftLib/Network/Packet.h>
#include <MinecraftLib/Network/Event.h>
#include <MinecraftLib/Component/Tags.h>
#include <MinecraftLib/Component/Prefab.h>
#include <MinecraftLib/Component/Input.h>
#include <MinecraftLib/Utils/LambdaAsFuncPtr.h>

#include <fmt/format.h>

#include <csignal>
#include <glm/ext/quaternion_trigonometric.hpp>

namespace Mcc
{

	ServerApplication::ServerApplication() :
		ServerApplication(0, nullptr)
	{}

	ServerApplication::ServerApplication(int argc, char** argv) :
		Application(argc, argv), mInfo({ DefaultTickRate }), mNetworkManager(mCmdLineStore)
	{
		CommandLineStore::OptParameter param;
		if ((param = mCmdLineStore.GetParameter("tick-rate").or_else([&]{ return mCmdLineStore.GetParameter("tr"); })).has_value())
		{
			unsigned long tickRate = std::stoul(param.value());
			if (tickRate < MinTickRate || tickRate > MaxTickRate) {
				fmt::print("TickRate must be between {} and {}, it was set at {}", MinTickRate, MaxTickRate, DefaultTickRate);
			}
			else
				mInfo.tickRate = tickRate;
		}

		mWorld.prefab<WorldEntityPrefab>()
		    .add<WorldEntity>()
			.set_auto_override(Position {{ 0.f, 2.f, -4.f }})
			.set_auto_override(Rotation {{ 0.f, 0.f }})
			.set_auto_override(Forward  {{ 0.f, 0.f, 0.f }})
			.set_auto_override(Right 	{{ 0.f, 0.f, 0.f }});

		auto lookupQuery = mWorld.query_builder<const Position, const Rotation>().with<WorldEntity>().build();
		mNetworkManager.Subscribe<ConnectEvent>([this, lookupQuery](const auto& event) {
			OnConnection packet;
			packet.initialStates = {};

			lookupQuery
				.run([&](flecs::iter& it) {
					while (it.next())
					{
						auto p = it.field<const Position>(0);
						auto r = it.field<const Rotation>(1);

						for (auto i: it)
						{
							auto entity = it.entity(i);
							packet.initialStates.push_back({ mLocalToNetwork[entity.id()], p[i], r[i], std::nullopt });
						}
					}
				});

			auto id		= GenerateNetworkID();
			auto entity = mWorld.entity().is_a<WorldEntityPrefab>();
			event.peer->data = new PlayerInfo { id };

			mLocalToNetwork.emplace(entity.id(), id);
			mNetworkToLocal.emplace(id, entity.id());

			packet.playerInfo = *static_cast<PlayerInfo*>(event.peer->data);
			packet.serverInfo = mInfo;

			mNetworkManager.Send(event.peer, std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
			entity.add<NewWorldEntity>();
		});

		mNetworkManager.Subscribe<DisconnectEvent>([this](const auto& event) {
			auto* playerInfo = (PlayerInfo*)event.peer->data;
			mWorld.entity(mNetworkToLocal[playerInfo->id]).add<ToDelete>();
			delete playerInfo;
		});

		mNetworkManager.Subscribe<From<OnPlayerInput>>([this](const From<OnPlayerInput>& event) {
			auto playerInfo = static_cast<PlayerInfo*>(event.peer->data);
			if (mWorld.exists(mNetworkToLocal[playerInfo->id]))
			{
				auto entity = mWorld.entity(mNetworkToLocal[playerInfo->id]);
				if (entity.has<PlayerInputQueue>())
				{
					entity.get_ref<PlayerInputQueue>()->push_back(event.packet.input);
				}
				else
				{
					entity.set(PlayerInputQueue {{event.packet.input}});
				}
			}
		});

		mWorld.system()
			.with<ToDelete>()
		    .run([this](flecs::iter& it) {
				while (it.next())
				{
					OnEntitiesDestroyed packet;
					for (auto i: it)
					{
						flecs::entity entity = it.entity(i);
						packet.ids.push_back(mLocalToNetwork[entity.id()]);

						mNetworkToLocal.erase(mLocalToNetwork[entity.id()]);
						mLocalToNetwork.erase(entity.id());

						entity.destruct();
					}
					mNetworkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
				}
			});

		mWorld.system<const Position, const Rotation>()
			.with<NewWorldEntity>()
			.run([this](flecs::iter& it) {
				while (it.next())
				{
					auto p = it.field<const Position>(0);
					auto r = it.field<const Rotation>(1);

					OnWorldEntitiesCreated packet;
					for (auto i: it)
					{
						flecs::entity entity = it.entity(i);
						packet.states.push_back({mLocalToNetwork[entity.id()], p[i], r[i], std::nullopt});
						entity.remove<NewWorldEntity>();
					}
					mNetworkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
				}
			});

		mWorld.system<PlayerInputQueue, Position, Rotation, Forward, Right>()
		    .each([](flecs::entity entity, PlayerInputQueue& inputs, Position& pos, Rotation& rot, Forward& forward, Right& right) {
				const float speed = 5.f;

				if (inputs.empty())
				{
					entity.remove<LastInputTracker>();
					return;
				}

				for (; !inputs.empty(); inputs.pop_front())
				{
					auto& input = inputs.front();
					input.Apply(rot, forward, right);
					input.Apply(input.meta.dt, speed, pos, forward, right);

					if (inputs.size() == 1)
					{
						auto rLit = entity.get_ref<LastInputTracker>();
						if (rLit.has())
						{
							rLit->id = input.meta.id;
						}
						else
						{
							entity.set<LastInputTracker>({ input.meta.id });
						}
					}
				}

				entity.add<WorldEntityUpdated>();
			});

		mWorld.system<const Position, const Rotation>()
		    .with<WorldEntityUpdated>()
		    .run([this](flecs::iter& it) {
				while (it.next())
				{
					auto p = it.field<const Position>(0);
					auto r = it.field<const Rotation>(1);

					OnEntitiesUpdated packet;
					for (auto i: it)
					{
						flecs::entity entity = it.entity(i);

						EntityState state;
						state.id	   = mLocalToNetwork[entity.id()];
						state.position = p[i];
						state.rotation = r[i];
						state.lastInputApplied = entity.has<LastInputTracker>() ? std::optional(entity.get_ref<LastInputTracker>()->id) : std::nullopt;

						packet.states.push_back(state);
						entity.remove<WorldEntityUpdated>();
					}
					mNetworkManager.Broadcast(std::move(packet), ENET_PACKET_FLAG_RELIABLE, 0);
				}
			});
	}

	int ServerApplication::Run()
	{
		auto quit = LambdaAsFuncPtr<void(*)(int)>([&](int) -> void { mWorld.quit(); });
		signal(SIGINT, quit);

		int state;
		if ((state = mNetworkManager.Setup()))
			return state;

		mWorld.set_target_fps(mInfo.tickRate);
		while (mWorld.progress())
		{
			mNetworkManager.Poll();
		}

		return EXIT_SUCCESS;
	}

}