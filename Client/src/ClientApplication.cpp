//
// Created by Alex on 02/09/2024.
//

#include <Client/ClientApplication.h>
#include <Client/Component/Tags.h>

#include <MinecraftLib/Network/Event.h>
#include <MinecraftLib/Utils/LambdaAsFuncPtr.h>
#include <MinecraftLib/PlayerInput.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <csignal>

namespace Mcc
{

	ClientApplication::ClientApplication() :
		ClientApplication(0, nullptr)
	{}

	ClientApplication::ClientApplication(int argc, char** argv) :
		Application(argc, argv),
		mNetworkManager(mCmdLineStore),
		mHasPlayerInfo(false),
		mPlayerInfo({}),
		mHasServerInfo(false),
		mServerInfo({}),
		mWindow("Minecraft"),
		mPlayerInput({}),
		mRenderSystem(mWorld, mPlayerInfo, mWindow)
	{

		mWindow.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mWindow.Subscribe<KeyEvent>([this](const KeyEvent& e) {
			if (e.action == GLFW_PRESS || e.action == GLFW_RELEASE)
			{
				switch (e.key)
				{
					case GLFW_KEY_W:
						mPlayerInput.movement.forward = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_S:
						mPlayerInput.movement.backward = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_A:
						mPlayerInput.movement.left = e.action == GLFW_PRESS;
						break;
					case GLFW_KEY_D:
						mPlayerInput.movement.right = e.action == GLFW_PRESS;
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

		mNetworkManager.Subscribe<OnPlayerInfo>([this](const auto& packet) {
			mHasPlayerInfo = true;
			mPlayerInfo    = packet.info;
		});

		mNetworkManager.Subscribe<OnServerInfo>([this](const auto& packet) {
			mHasServerInfo = true;
			mServerInfo    = packet.info;
	   });

		mNetworkManager.Subscribe<OnWorldEntitiesCreated>([this](const OnWorldEntitiesCreated& e) {
			for (auto& state: e.states)
			{
				assert(!mWorld.is_alive(state.entity));
				auto entity = mWorld.make_alive(state.entity)
					.set(state.position)
					.set(state.rotation);

				entity.set(Forward {glm::vec3(
					cos(state.rotation.vec.y) * sin(state.rotation.vec.x),
					sin(state.rotation.vec.y),
					cos(state.rotation.vec.y) * cos(state.rotation.vec.x)
				)});

				entity.set(Right { glm::vec3(
					sin(state.rotation.vec.x - 3.14f / 2.0f),
					0,
					cos(state.rotation.vec.x - 3.14f / 2.0f)
				)});

				if (entity.id() == mPlayerInfo.entity)
				{
					entity.add<Controlled>();
				}
			}
		});

		mNetworkManager.Subscribe<OnEntitiesDestroyed>([this](const OnEntitiesDestroyed& e) {
			for (auto& entity: e.entities)
			{
				if (mWorld.is_alive(entity))
				{
					mWorld.entity(entity).destruct();
				}
			}
		});

		mNetworkManager.Subscribe<OnEntitiesUpdated>([this](const OnEntitiesUpdated& event) {
			for (auto& state: event.states)
			{
				if (mWorld.exists(state.entity))
				{
					flecs::entity entity = mWorld.entity(state.entity);
					entity.set(state.position);
					entity.set(state.rotation);

					entity.set(Forward {glm::vec3(
						cos(state.rotation.vec.y) * sin(state.rotation.vec.x),
						sin(state.rotation.vec.y),
						cos(state.rotation.vec.y) * cos(state.rotation.vec.x)
					)});

					entity.set(Right { glm::vec3(
						sin(state.rotation.vec.x - 3.14f / 2.0f),
						0,
						cos(state.rotation.vec.x - 3.14f / 2.0f)
					)});
				}
			}
		});
	}

	void ClientApplication::SetupWorld()
	{
		flecs::entity networkTick = mWorld.timer().interval(1.f / static_cast<float>(mServerInfo.tickRate));
		mWorld.system()
			.tick_source(networkTick)
			.run([this](flecs::iter&) {
				mNetworkManager.Send<OnPlayerInput>({ mPlayerInput }, ENET_PACKET_FLAG_RELIABLE, 0);
				mPlayerInput.axis.x = 0;
				mPlayerInput.axis.y = 0;
			});
	}

	int ClientApplication::Run()
	{
		auto quit = LambdaAsFuncPtr<void(*)(int)>([&](int) -> void { mWorld.quit(); });
		signal(SIGTSTP, quit);
		signal(SIGINT , quit);
		signal(SIGABRT, quit);

		if (int error = mNetworkManager.Setup())
			return error;
		if (int error = mNetworkManager.Connect())
			return error;

		while (!mHasPlayerInfo && !mHasServerInfo)
		{
			mNetworkManager.Poll();
		}

		SetupWorld();

		while (mWorld.progress() && !mWindow.ShouldClose())
		{
			mNetworkManager.Poll();
		}

		return mNetworkManager.Disconnect();
	}

}