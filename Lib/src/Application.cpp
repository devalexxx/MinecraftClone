//
// Created by Alex on 02/09/2024.
//

#include <MinecraftLib/Application.h>

#include <csignal>
#include <thread>

namespace Mcc
{

	static void* SigHandler(void* args)
	{
		auto [set, world] = *static_cast<std::tuple<sigset_t, flecs::world&>*>(args);
		int sig;
		sigwait(&set, &sig);
		if (sig == SIGINT || sig == SIGABRT)
		{
			world.quit();
		}
		return nullptr;
	}

	Application::Application(int argc, char** argv) :
		mCmdLineStore(argc, argv)
	{
#if MCC_DEBUG
		mWorld.import<flecs::stats>();
		CommandLineStore::OptParameter param;;
		if ((param = mCmdLineStore.GetParameter("fport").or_else([&]{ return mCmdLineStore.GetParameter("fp"); })).has_value())
			mWorld.set<flecs::Rest>({.port=static_cast<uint16_t>(std::stoi(param->get()))});
		else
			mWorld.set<flecs::Rest>({});
#endif

		sigset_t set;
		pthread_t thread;

		sigemptyset(&set);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGABRT);
		sigprocmask(SIG_BLOCK, &set, nullptr);

		std::tuple<sigset_t, flecs::world&> args(set, mWorld);
		pthread_create(&thread, nullptr, SigHandler, &args);
	}

}