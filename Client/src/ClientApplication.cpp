//
// Created by Alex on 02/09/2024.
//

#include <Client/ClientApplication.h>
#include <Client/Component/Tags.h>

#include <MinecraftLib/Network/Event.h>
#include <MinecraftLib/Utils/LambdaAsFuncPtr.h>

#include <glm/gtc/quaternion.hpp>

#include "Client/World/Context.h"
#include <csignal>

namespace Mcc
{

	ClientApplication::ClientApplication() :
		ClientApplication(0, nullptr)
	{}

	ClientApplication::ClientApplication(int argc, char** argv) :
		Application(argc, argv),
		mNetworkManager(mCmdLineStore),
		mConnected(false),
		mPlayerInfo({}),
		mServerInfo({}),
		mWindow("Minecraft"),
		mInput({})
	{

//		mWindow.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mWindow.Subscribe<KeyEvent>([this](const KeyEvent& e) {
			if (e.action == GLFW_PRESS || e.action == GLFW_RELEASE)
			{
				switch (e.key)
				{
					case GLFW_KEY_W:
						mInput.movement.forward = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_S:
						mInput.movement.backward = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_A:
						mInput.movement.left = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_D:
						mInput.movement.right = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_ESCAPE:
						e.window.SetShouldClose();
					default:
						break;
				}
			}
		});

		mWindow.Subscribe<CursorPosEvent>([this](const CursorPosEvent e) {
			auto sensitivity = 0.001f;
			auto [w, h] = e.window.GetWindowSize();

			if (e.x >= 0 && e.x <= w && e.y >= 0 && e.y <= h && e.window.IsFocused())
			{
				mWindow.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

				mInput.axis.x += sensitivity * ((static_cast<float>(w) / 2.f) - static_cast<float>(e.x));
				mInput.axis.y += sensitivity * ((static_cast<float>(h) / 2.f) - static_cast<float>(e.y));

				e.window.SetCursorPosition(w / 2, h / 2);
			}
		});

		mNetworkManager.Subscribe<MalformedPacketEvent>([](const auto&) {
			fmt::print("Handle malformed packet\n");
		});

		mNetworkManager.Subscribe<DisconnectEvent>([&](const auto&) {
			mWorld.quit();
		});

		mNetworkManager.Subscribe<OnConnection>([this](const OnConnection& packet) {
			mPlayerInfo = packet.playerInfo;
			mServerInfo = packet.serverInfo;

			for (auto& state: packet.initialStates)
			{
				auto entity = mWorld.entity()
				  .set(state.position)
				  .set(state.rotation)
				  .set(Forward::FromRotation(state.rotation))
				  .set(Right  ::FromRotation(state.rotation));

				mLocalToNetwork.emplace(entity.id(), state.id);
				mNetworkToLocal.emplace(state.id, entity.id());
			}

			mWorld.set_ctx(new ClientWorldContext { { mServerInfo, mNetworkToLocal, mLocalToNetwork }, mPlayerInfo, mWindow }, [](void* ptr) { delete static_cast<ClientWorldContext*>(ptr); });

			mConnected = true;
	    });

		mNetworkManager.Subscribe<OnWorldEntitiesCreated>([this](const OnWorldEntitiesCreated& e) {
			for (auto& state: e.states)
			{
				auto entity = mWorld.entity()
					.set(state.position)
					.set(state.rotation)
					.set(Forward::FromRotation(state.rotation))
					.set(Right  ::FromRotation(state.rotation));

				mLocalToNetwork.emplace(entity.id(), state.id);
				mNetworkToLocal.emplace(state.id, entity.id());

				if (state.id == mPlayerInfo.id)
				{
					entity.add<Controlled>();
				}
			}
		});

		mNetworkManager.Subscribe<OnEntitiesDestroyed>([this](const OnEntitiesDestroyed& e) {
			for (auto& id: e.ids)
			{
				mWorld.entity(mNetworkToLocal[id]).destruct();

				mLocalToNetwork.erase(mNetworkToLocal[id]);
				mNetworkToLocal.erase(id);
			}
		});

		mNetworkManager.Subscribe<OnEntitiesUpdated>([this](const OnEntitiesUpdated& event) {
			for (auto& state: event.states)
			{
				auto id = mNetworkToLocal[state.id];
				if (mWorld.exists(id))
				{
					flecs::entity entity = mWorld.entity(id);

					entity.set(state.position);
					entity.set(state.rotation);
					entity.set(Forward::FromRotation(state.rotation));
					entity.set(Right  ::FromRotation(state.rotation));

					if (entity.has<Controlled>() && state.lastInputApplied.has_value())
					{
						// Drop inputs already processed by the server
						auto iid = *state.lastInputApplied;
						for (; !mInputQueue.empty(); mInputQueue.pop_front())
						{
							auto& input = mInputQueue.front();
							if (input.meta.id == iid)
							{
								mInputQueue.pop_front();
								break;
							}
						}

						if (!mInputQueue.empty())
							assert(mInputQueue.front().meta.id - iid == 1);

						// Reapply all input unprocessed by the server
						for (auto& input: mInputQueue)
						{
							const float speed = 5.f;
							entity.get([&input, speed](Position& position, Rotation& rotation, Forward& forward, Right& right) {
								input.Apply(rotation, forward, right);
								input.Apply(input.meta.dt, speed, position, forward, right);
							});
						}

					}
				}
			}
		});
	}

	void ClientApplication::SetupWorld()
	{
//		mWorld.system()
//			.run([](flecs::iter& it) {
//				static float elapsed = 0;
//				static float frames  = 0;
//
//				elapsed += it.delta_time();
//				frames	+= 1;
//
//				if (elapsed >= 1)
//				{
//					fmt::print("fps: {}\n", frames);
//					frames  = 0;
//					elapsed = 0;
//				}
//			});

		mWorld.import<Renderer>();
//		mWorld.import<flecs::stats>();
//
//		CommandLineStore::OptParameter param;;
//		if ((param = mCmdLineStore.GetParameter("fport").or_else([&]{ return mCmdLineStore.GetParameter("fp"); })).has_value())
//			mWorld.set<flecs::Rest>({.port=static_cast<uint16_t>(std::stoi(param->get()))});
//		else
//			mWorld.set<flecs::Rest>({});

		mWorld.system<Position, Rotation, Forward, Right>()
			.with<Controlled>()
			.each([this](flecs::iter& it, size_t, Position& position, Rotation& rotation, Forward& forward, Right& right) {
				if (mInput.NotNull())
				{
					const float speed = 5.f;
					const float delta = it.delta_time();

					mInput.meta.id = PlayerInputMeta::GetNextID();
					mInput.meta.dt = delta;
					mNetworkManager.Send<OnPlayerInput>({ mInput }, ENET_PACKET_FLAG_RELIABLE, 0);

					mInput.Apply(rotation, forward, right);
					mInput.Apply(delta, speed, position, forward, right);

					mInputQueue.push_back(mInput);
					mInput.axis = {};
				}
			});

	}

	int ClientApplication::Run()
	{
		auto quit = LambdaAsFuncPtr<void(*)(int)>([&](int) -> void { mWorld.quit(); });
		signal(SIGINT , quit);
		signal(SIGABRT, quit);

		if (int error = mNetworkManager.Setup())
			return error;
		if (int error = mNetworkManager.Connect())
			return error;

		while (!mConnected)
		{
			mNetworkManager.Poll();
		}

		SetupWorld();

		while (!mWorld.should_quit() && !mWindow.ShouldClose())
		{
			mWorld.progress();
			mNetworkManager.Poll();
		}

		return mNetworkManager.Disconnect();
	}

}