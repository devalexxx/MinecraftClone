//
// Created by Alex on 02/09/2024.
//

#include <MinecraftLib/Application.h>

namespace Mcc
{

	Application::Application(int argc, char** argv) :
		mCmdLineStore(argc, argv)
	{
#if MC_DEBUG
		mWorld.import<flecs::stats>();
		CommandLineStore::OptParameter param;;
		if ((param = mCmdLineStore.GetParameter("fport").or_else([&]{ return mCmdLineStore.GetParameter("fp"); })).has_value())
			mWorld.set<flecs::Rest>({.port=static_cast<uint16_t>(std::stoi(param->get()))});
		else
			mWorld.set<flecs::Rest>({});
#endif
	}

}