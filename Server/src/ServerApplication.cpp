//
// Created by Alex on 02/09/2024.
//

#include "Server/ServerApplication.h"
#include "../include/Server/WorldContext.h"
#include "Server/Module/EntityReplication/Module.h"
#include "Server/Module/Player/Module.h"
#include "Server/Module/UserSession/Module.h"

#include "Common/Module/Entity/Module.h"
#include "Common/Module/Network/Module.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Module/Terrain/Module.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Logging.h"
#include "Server/Module/TerrainReplication/Module.h"
#include "Server/World/ChunkGenerator.h"

#include <fmt/format.h>

#include <charconv>
#include <utility>

namespace Mcc
{

	ServerApplication::ServerApplication() :
		ServerApplication(0, nullptr)
	{}

	ServerApplication::ServerApplication(int argc, char** argv) :
		Application(argc, argv), mInfo({ DefaultTickRate }), mNetworkManager(mCmdLineStore)
	{
		if (CommandLineStore::OptParameter param; (param = mCmdLineStore.GetParameter("tick-rate").or_else([&]{ return mCmdLineStore.GetParameter("tr"); })).has_value())
		{
			unsigned long tickRate;
			std::from_chars(param->cbegin(), param->cend(), tickRate);
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
		mWorld.set_ctx(new ServerWorldContext { { mInfo, mNetworkManager, {}, mThreadPool } }, [](void* ptr) { delete static_cast<ServerWorldContext*>(ptr); });
	    mWorld.import<NetworkModule>          ();
		mWorld.import<EntityModule>		 	  ();
		mWorld.import<UserSessionModule>	  ();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerModule>			  ();
		mWorld.import<TerrainModule>		  ();
		mWorld.import<TerrainReplicationModule>();

	    mWorld.add<ServerTag>();

		{
			mWorld.entity("mcc:block:air")
		        .is_a<BlockPrefab>()
				.set<BlockType>(BlockType::Gas)
				.set<BlockMeta>({ "mcc:block:air" });

			mWorld.entity("mcc:block:stone")
                .is_a<BlockPrefab>()
				.set<BlockType>(BlockType::Solid)
		        .set<BlockColor>({ { .5f, .5f, .5f } })
				.set<BlockMeta>({ "mcc:block:stone" });

		    mWorld.entity("mcc:block:dirt")
                .is_a<BlockPrefab>()
                .set<BlockType>(BlockType::Solid)
                .set<BlockColor>({ { .0f, .7f, .3f } })
                .set<BlockMeta>({ "mcc:block:dirt" });


            const ChunkGenerator gen(mWorld, 12345u);
            constexpr int size = 13;
            for (int x = -size; x <= size; ++x)
            {
                for (int z = -size; z <= size; ++z)
                {
                    mWorld.entity()
                        .is_a<ChunkPrefab>()
                        .set<ChunkPosition>({ { x, 0, z } })
                        .set<ChunkHolder>({ std::make_shared<Chunk>(gen.Generate({ x + size, 0, z + size })) });
                }
            }
		}

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