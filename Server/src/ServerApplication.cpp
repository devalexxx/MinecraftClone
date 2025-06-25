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

#include <fmt/format.h>

#include "MinecraftLib/Utils/Logging.h"

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
				MCC_LOG_WARN("TickRate must be between {} and {}, it was set at {}", MinTickRate, MaxTickRate, DefaultTickRate);
			}
			else
				mInfo.tickRate = tickRate;
		}
	}

	int ServerApplication::Run()
	{
		if (int state = mNetworkManager.Setup())
		{
			MCC_LOG_ERROR("Failed to create the network host");
			return state;
		}

		MCC_LOG_DEBUG("Setup world...");
		mWorld.set_ctx(new ServerWorldContext { { mInfo, mNetworkManager, {}, {} } }, [](void* ptr) { delete static_cast<ServerWorldContext*>(ptr); });
		mWorld.import<WorldEntityModule> 	  ();
		mWorld.import<PlayerEntityModule>	  ();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerSessionModule>	  ();
		mWorld.import<PlayerInputModule>	  ();

		MCC_LOG_INFO("Application started and listening on port {}", mNetworkManager.mAddr.port);
		mWorld.set_target_fps(mInfo.tickRate);
		while (!mWorld.should_quit())
		{
			mWorld.progress();
			mNetworkManager.Poll();
		}

		MCC_LOG_INFO("Shutdown...");

		return EXIT_SUCCESS;
	}

}