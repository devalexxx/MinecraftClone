//
// Created by Alex on 02/09/2024.
//

#include "Server/ServerApplication.h"
#include "Server/Module/EntityReplication/Module.h"
#include "Server/Module/Player/Module.h"
#include "Server/Module/UserSession/Module.h"
#include "Server/World/Context.h"

#include "Common/Module/Entity/Module.h"
#include "Common/Module/Terrain/Component.h"
#include "Common/Module/Terrain/Module.h"
#include "Common/Network/Event.h"
#include "Common/Network/Packet.h"
#include "Common/Utils/Logging.h"
#include "Server/Module/TerrainReplication/Module.h"

#include <fmt/format.h>

#include <charconv>

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
		mWorld.set_ctx(new ServerWorldContext { { mInfo, mNetworkManager, {}, {} } }, [](void* ptr) { delete static_cast<ServerWorldContext*>(ptr); });
		mWorld.import<EntityModule>		 	  ();
		mWorld.import<UserSessionModule>	  ();
		mWorld.import<EntityReplicationModule>();
		mWorld.import<PlayerModule>			  ();
		mWorld.import<TerrainModule>		  ();
		mWorld.import<TerrainReplicationModule>();

		{
			auto* ctx = static_cast<ServerWorldContext*>(mWorld.get_ctx());
			flecs::entity e;
			NetworkID 	  id;
			e = mWorld.entity("mcc:block:air")
				.add<BlockTag>()
				.set<BlockMeta>({ "mcc:block:air" });
			id = GenerateNetworkID();
			ctx->localToNetwork.emplace(e.id(), id);
			ctx->networkToLocal.emplace(id, e.id());

			e = mWorld.entity("mcc:block:stone")
				.add<BlockTag>()
				.set<BlockMeta>({ "mcc:block:stone" });
			id = GenerateNetworkID();
			ctx->localToNetwork.emplace(e.id(), id);
			ctx->networkToLocal.emplace(id, e.id());
			
			e = mWorld.entity()
				.add<ChunkTag>()
				.set<ChunkPosition>({ { 0, 0, 0 } })
				.set<ChunkData>({ std::make_unique<Chunk>(mWorld.lookup("mcc:block:air")) });
			id = GenerateNetworkID();
			ctx->localToNetwork.emplace(e.id(), id);
			ctx->networkToLocal.emplace(id, e.id());

			auto cData = e.get_ref<ChunkData>();
			auto stone = mWorld.lookup("mcc:block:stone");

			for (int x = 0; x < Chunk::Size; ++x)
			{
				for (int z = 0; z < Chunk::Size; ++z)
				{
					for (int y = 0; y < 12; ++y)
					{
						cData->data->Set({ x, y, z }, stone);
					}
				}
			}

			e = mWorld.entity()
					.add<ChunkTag>()
					.set<ChunkPosition>({ { 1, 0, 0 } })
					.set<ChunkData>({ std::make_unique<Chunk>(mWorld.lookup("mcc:block:air")) });
			id = GenerateNetworkID();
			ctx->localToNetwork.emplace(e.id(), id);
			ctx->networkToLocal.emplace(id, e.id());

			cData = e.get_ref<ChunkData>();

			for (int x = 0; x < Chunk::Size; ++x)
			{
				for (int z = 0; z < Chunk::Size; ++z)
				{
					for (int y = 0; y < 5; ++y)
					{
						cData->data->Set({ x, y, z }, stone);
					}
				}
			}

			e = mWorld.entity()
					.add<ChunkTag>()
					.set<ChunkPosition>({ { 0, 0, 1 } })
					.set<ChunkData>({ std::make_unique<Chunk>(mWorld.lookup("mcc:block:air")) });
			id = GenerateNetworkID();
			ctx->localToNetwork.emplace(e.id(), id);
			ctx->networkToLocal.emplace(id, e.id());

			cData = e.get_ref<ChunkData>();

			for (int x = 0; x < Chunk::Size; ++x)
			{
				for (int z = 0; z < Chunk::Size; ++z)
				{
					for (int y = 0; y < 16; ++y)
					{
						cData->data->Set({ x, y, z }, stone);
					}
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