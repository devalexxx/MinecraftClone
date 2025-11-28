// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_SCENE_IMPORTER_H
#define MCC_COMMON_SCENE_IMPORTER_H

#include <Hexis/Core/TypeList.h>

#include <flecs.h>

namespace Mcc
{

    namespace _
    {
        struct Load
        {};
        struct Main
        {};
        struct Quit
        {};

        struct Global
        {};
    }

    struct ActiveScene
    {};
    struct SceneRoot
    {};

    template<typename...>
    struct Pipeline
    {};

    struct LoadPipeline : Pipeline<_::Global, _::Load>
    {};
    struct QuitPipeline : Pipeline<_::Global, _::Quit>
    {};

    template<typename T, typename M>
    struct Scene
    {
        struct LoadPipeline : Pipeline<T, _::Load>
        {};
        struct MainPipeline : Pipeline<T, _::Main>
        {};
        struct QuitPipeline : Pipeline<T, _::Quit>
        {};

        static void Register(const flecs::world& world);
        static void OnLoad(flecs::iter& it);
        static void OnQuit(flecs::iter& it);
    };

    flecs::component<SceneRoot> GetSceneRoot(const flecs::world& world);

    template<typename... Ts>
    struct SceneImporter
    {
        using SceneList    = Hx::TypeList<Ts...>;
        using DefaultScene = SceneList::template TypeAt<0>;

        explicit SceneImporter(flecs::world& world);
    };

    template<typename T>
    struct SceneModule
    {};

}

#include "Common/SceneImporter.inl"

#endif
