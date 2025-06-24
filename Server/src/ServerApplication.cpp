//
// Created by Alex on 02/09/2024.
//

#include <Server/ServerApplication.h>
#include <Server/Module/EntityReplication/Module.h>
#include <Server/Module/PlayerInput/Module.h>
#include <Server/Module/PlayerSession/Module.h>
#include <Server/World/Context.h>

#include <MinecraftLib/Network/Packet.h>
#include <MinecraftLib/Network/Event.h>
#include <MinecraftLib/Module/PlayerEntity/Component.h>
#include <MinecraftLib/Module/WorldEntity/Module.h>
#include <MinecraftLib/Utils/LambdaAsFuncPtr.h>

#include <fmt/format.h>

#include <csignal>

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
	}

	int ServerApplication::Run()
	{
		auto quit = LambdaAsFuncPtr<void(*)(int)>([&](int) -> void { mWorld.quit(); });
		signal(SIGINT, quit);

		int state;
		if ((state = mNetworkManager.Setup()))
			return state;

		mWorld.set_ctx(new ServerWorldContext { { mInfo, mNetworkManager, {}, {} } }, [](void* ptr) { delete static_cast<ServerWorldContext*>(ptr); });
		mWorld.import<WorldEntityModule> 	  ();
		mWorld.import<PlayerEntityModule>	  ();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerSessionModule>	  ();
		mWorld.import<PlayerInputModule>	  ();

		mWorld.set_target_fps(mInfo.tickRate);
		while (!mWorld.should_quit())
		{
			mWorld.progress();
			mNetworkManager.Poll();
		}

		return EXIT_SUCCESS;
	}

}