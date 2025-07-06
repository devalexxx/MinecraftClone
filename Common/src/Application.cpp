//
// Created by Alex on 02/09/2024.
//

#include "Common/Application.h"

#include <charconv>
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
		if (const auto param = mCmdLineStore.GetParameter("fport").or_else([&]{ return mCmdLineStore.GetParameter("fp"); }); param.has_value())
		{
			flecs::Rest config;
			std::from_chars(param->cbegin(), param->cend(), config.port);;
			mWorld.set<flecs::Rest>(config);
		}
		else
		{
			mWorld.set<flecs::Rest>({});
		}
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