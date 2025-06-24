//
// Created by Alex on 02/09/2024.
//

#include <Client/ClientApplication.h>
#include <Client/World/Context.h>
#include <Client/Module/PlayerSession/Module.h>
#include <Client/Module/PlayerSession/Component.h>
#include <Client/Module/EntityReplication/Module.h>
#include <Client/Module/PlayerInput/Module.h>

#include <MinecraftLib/Network/Event.h>
#include <MinecraftLib/Module/PlayerEntity/Module.h>
#include <MinecraftLib/Module/WorldEntity/Module.h>

namespace Mcc
{

	ClientApplication::ClientApplication() :
		ClientApplication(0, nullptr)
	{}

	ClientApplication::ClientApplication(int argc, char** argv) :
		Application(argc, argv),
		mNetworkManager(mCmdLineStore),
		mWindow("Minecraft")
	{
		mNetworkManager.Subscribe<MalformedPacketEvent>([](const auto&) {
			fmt::print("Handle malformed packet\n");
		});
	}

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

	int ClientApplication::Run()
	{
		if (int error = mNetworkManager.Setup())
			return error;
		if (int error = mNetworkManager.Connect())
			return error;

		mWorld.set_ctx(new ClientWorldContext { { {}, mNetworkManager, {}, {} }, {}, mWindow }, [](void* ptr) { delete static_cast<ClientWorldContext*>(ptr); });
		mWorld.import<PlayerSessionModule>();

		while (*mWorld.get<ServerConnectionState>() != ServerConnectionState::Connected)
		{
			mNetworkManager.Poll();
		}

		mWorld.import<WorldEntityModule>();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerEntityModule>();
		mWorld.import<PlayerInputModule>();
		mWorld.import<Renderer>();

		while (!mWorld.should_quit() && !mWindow.ShouldClose())
		{
			mWorld.progress();
			mNetworkManager.Poll();
		}

		return mNetworkManager.Disconnect();
	}

}