//
// Created by Alex on 02/09/2024.
//

#include "Server/ServerApplication.h"
#include "Server/Module/EntityReplication/Module.h"
#include "Server/Module/Player/Module.h"
#include "Server/Module/UserSession/Module.h"
#include "Server/World/Context.h"

#include "Common/Network/Packet.h"
#include "Common/Network/Event.h"
#include "Common/Module/Entity/Module.h"
#include "Common/Utils/Logging.h"

#include <fmt/format.h>

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
		mWorld.import<EntityModule>		 	  ();
		mWorld.import<UserSessionModule>	  ();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerModule>			  ();

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