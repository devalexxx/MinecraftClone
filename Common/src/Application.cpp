//
// Created by Alex on 02/09/2024.
//

#include "Common/Application.h"

#include "Common/Utils/Logging.h"

#include <charconv>
#include <csignal>
#include <tuple>
#include <thread>

namespace Mcc
{

#ifdef MCC_POSIX
	static void* SigHandler(void* args)
	{
		auto [set, world] = *static_cast<std::tuple<sigset_t, flecs::world>*>(args);
		int sig;
		sigwait(&set, &sig);
		if (sig == SIGINT || sig == SIGABRT)
		{
			world.quit();
		}
		return nullptr;
	}
#endif

	Application::Application(int argc, char** argv) :
		mCmdLineStore(argc, argv)
#ifdef MCC_POSIX
		,mArgs        ({}, mWorld)
#endif
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

#ifdef MCC_POSIX
		sigset_t set;
		pthread_t thread;

		sigemptyset(&set);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGABRT);
		sigprocmask(SIG_BLOCK, &set, nullptr);

        std::get<sigset_t>(mArgs) = set;
		pthread_create(&thread, nullptr, SigHandler, &mArgs);
#endif
	}

}