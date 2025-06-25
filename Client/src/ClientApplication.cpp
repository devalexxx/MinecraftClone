//
// Created by Alex on 02/09/2024.
//

#include "Client/ClientApplication.h"
#include "Client/World/Context.h"
#include "Client/Module/PlayerSession/Module.h"
#include "Client/Module/PlayerSession/Component.h"
#include "Client/Module/EntityReplication/Module.h"
#include "Client/Module/PlayerInput/Module.h"

#include "Common/Utils/Logging.h"
#include "Common/Module/PlayerEntity/Module.h"
#include "Common/Module/WorldEntity/Module.h"
#include "Common/Network/Event.h"

namespace Mcc
{

	ClientApplication::ClientApplication() :
		ClientApplication(0, nullptr)
	{}

	ClientApplication::ClientApplication(int argc, char** argv) :
		Application(argc, argv),
		mNetworkManager(mCmdLineStore),
		mWindow("MachinaCubicaCatalyst")
	{
		mNetworkManager.Subscribe<MalformedPacketEvent>([](const auto&) { MCC_LOG_WARN("Handle malformed packet"); });
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
		{
			MCC_LOG_ERROR("Failed to setup network host");
			return error;
		}

		MCC_LOG_INFO("Waiting for connection...");
		if (int error = mNetworkManager.Connect())
		{
			MCC_LOG_ERROR("Failed to connect to server");
			return error;
		}

		MCC_LOG_DEBUG("Waiting for server information...");
		mWorld.set_ctx(new ClientWorldContext { { {}, mNetworkManager, {}, {} }, {}, mWindow }, [](void* ptr) { delete static_cast<ClientWorldContext*>(ptr); });
		mWorld.import<WorldEntityModule>();
		mWorld.import<PlayerSessionModule>();

		while (*mWorld.get<ServerConnectionState>() == ServerConnectionState::Pending)
		{
			mNetworkManager.Poll();
		}

		if (*mWorld.get<ServerConnectionState>() == ServerConnectionState::Error)
		{
			MCC_LOG_ERROR("Failed to retrieve server information");
			return EXIT_FAILURE;
		}

		MCC_LOG_DEBUG("Setup world...");
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerEntityModule>();
		mWorld.import<PlayerInputModule>();
		mWorld.import<Renderer>();

		MCC_LOG_INFO("Application started");

		while (!mWorld.should_quit() && !mWindow.ShouldClose())
		{
			mWorld.progress();
			mNetworkManager.Poll();
		}

		MCC_LOG_INFO("Shutdown...");

		if (int error = mNetworkManager.Disconnect())
			return error;

		MCC_LOG_INFO("Disconnected from server");

		return EXIT_SUCCESS;
	}

}