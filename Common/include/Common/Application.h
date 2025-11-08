// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_APPLICATION_H
#define MCC_COMMON_APPLICATION_H

#include "Common/CommandLineStore.h"

#include <enet/enet.h>

#include <flecs.h>

#include <unordered_map>

namespace Mcc
{

    class Application
    {
      public:
        Application(int argc, char** argv);
        virtual ~Application() = default;

        virtual int Run() = 0;

      protected:
        flecs::world     mWorld;
        CommandLineStore mCmdLineStore;

#ifdef MCC_POSIX
        std::tuple<sigset_t, flecs::world> mArgs;
#endif
    };

}

#endif
