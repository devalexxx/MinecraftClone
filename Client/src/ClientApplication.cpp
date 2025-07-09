//
// Created by Alex on 02/09/2024.
//

#include "Client/ClientApplication.h"
#include "Client/Module/Camera/Module.h"
#include "Client/Module/EntityRenderer/Module.h"
#include "Client/Module/EntityReplication/Module.h"
#include "Client/Module/Player/Module.h"
#include "Client/Module/Renderer/Module.h"
#include "Client/Module/ServerSession/Component.h"
#include "Client/Module/ServerSession/Module.h"
#include "Client/Module/TerrainRenderer/Module.h"
#include "Client/Module/TerrainReplication/Module.h"
#include "Client/World/Context.h"

#include "Common/Module/Entity/Module.h"
#include "Common/Module/Terrain/Module.h"
#include "Common/Network/Event.h"
#include "Common/Utils/Logging.h"

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
		mWorld.import<ServerSessionModule>();

		while (mWorld.get<ServerConnectionState>() == ServerConnectionState::Pending)
		{
			mNetworkManager.Poll();
		}

		if (mWorld.get<ServerConnectionState>() == ServerConnectionState::Error)
		{
			MCC_LOG_ERROR("Failed to retrieve server information");
			return EXIT_FAILURE;
		}

		MCC_LOG_DEBUG("Setup world...");
		mWorld.import<EntityModule>();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<CameraModule>();
		mWorld.import<PlayerModule>();
		mWorld.import<TerrainModule>();
		mWorld.import<TerrainReplicationModule>();
		mWorld.import<RendererModule>();
		mWorld.import<EntityRendererModule>();
		mWorld.import<TerrainRendererModule>();

		mWorld.system()
			.run([](flecs::iter& it) {
				static float elapsed = 0;
				static float frames  = 0;

				elapsed += it.delta_time();
				frames	+= 1;

				if (elapsed >= 1)
				{
					MCC_LOG_DEBUG("fps: {}", frames);
					frames  = 0;
					elapsed = 0;
				}
			});

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