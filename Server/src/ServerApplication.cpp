//
// Created by Alex on 02/09/2024.
//

#include "Server/ServerApplication.h"
#include "Server/WorldContext.h"
#include "Server/Module/EntityReplication/Module.h"
#include "Server/Module/Player/Module.h"
#include "Server/Module/UserSession/Module.h"
#include "Server/Module/TerrainReplication/Module.h"
#include "Server/World/ChunkGenerator.h"

#include "Common/Module/Entity/Module.h"
#include "Common/Module/Network/Module.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Module/Terrain/Module.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Logging.h"

#include <fmt/format.h>

#include <charconv>

namespace Mcc
{

	ServerApplication::ServerApplication() :
		ServerApplication(0, nullptr)
	{}

	ServerApplication::ServerApplication(int argc, char** argv) :
		Application(argc, argv),
        mSettings({
            .tickRate=TICK_RATE_DEFAULT,
            .renderDistance=RENDER_DISTANCE_DEFAULT,
            .userSpeed=USER_SPEED_DEFAULT
        }),
        mNetworkManager(mCmdLineStore)
	{
		if (CommandLineStore::OptParameter param; (param = mCmdLineStore.GetParameter("tick-rate").or_else([&]{ return mCmdLineStore.GetParameter("tr"); })).has_value())
		{
			unsigned long tickRate;
			std::from_chars(param->data(), param->data() + param->size(), tickRate);
			if (tickRate < TICK_RATE_MIN || tickRate > TICK_RATE_MAX)
			{
				MCC_LOG_WARN("TickRate must be between {} and {}, but was set to {}", TICK_RATE_MIN, TICK_RATE_MAX, TICK_RATE_DEFAULT);
			    tickRate = TICK_RATE_DEFAULT;
			}
		    mSettings.tickRate = tickRate;
		}

	    if (CommandLineStore::OptParameter param; (param = mCmdLineStore.GetParameter("render-distance").or_else([&]{ return mCmdLineStore.GetParameter("rd"); })).has_value())
	    {
	        unsigned long renderDistance;
	        std::from_chars(param->cbegin(), param->cend(), renderDistance);
	        if (renderDistance < RENDER_DISTANCE_MIN || renderDistance > RENDER_DISTANCE_MAX)
	        {
	            MCC_LOG_WARN("RenderDistance must be between {} and {}, but was set to {}", RENDER_DISTANCE_MIN, RENDER_DISTANCE_MAX, RENDER_DISTANCE_DEFAULT);
	            renderDistance = TICK_RATE_DEFAULT;
	        }
	        mSettings.renderDistance = renderDistance;
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
		mWorld.set_ctx(new ServerWorldContext { { mNetworkManager, {}, mThreadPool }, mSettings }, [](void* ptr) { delete static_cast<ServerWorldContext*>(ptr); });
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
            constexpr int size = 32;
            for (int x = -size; x <= size; ++x)
            {
                for (int z = -size; z <= size; ++z)
                {
                    mWorld.entity()
                        .is_a<ChunkPrefab>()
                        .set<ChunkPosition>({ { x, 0, z } })
                        .set<ChunkHolder>({ std::make_shared<Chunk>(gen.Generate({ x, 0, z })) });
                }
            }
		}

		MCC_LOG_INFO("Application started and listening on port {}", mNetworkManager.mAddr.port);
		mWorld.set_target_fps(mSettings.tickRate);
		while (!mWorld.should_quit())
		{
			mWorld.progress();
			mNetworkManager.Poll();
		}

		MCC_LOG_INFO("Shutdown...");

		return EXIT_SUCCESS;
	}

}