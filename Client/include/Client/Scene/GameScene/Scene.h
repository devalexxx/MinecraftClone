// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_SCENE_GAME_SCENE_SCENE_H
#define MCC_CLIENT_SCENE_GAME_SCENE_SCENE_H

#include "Client/Graphics/Window/Event.h"

#include "Common/Module/Base/Module.h"
#include "Common/SceneImporter.h"
#include "Common/State.h"

#include <flecs.h>

namespace Mcc
{

    REGISTER_STATE(StateTargetScene, GameState, Load, InGame, InMenu, Shutdown)

    struct GameScene;

    template<>
    struct SceneModule<GameScene> final : BaseModule<SceneModule<GameScene>>
    {
        SceneModule(flecs::world& world);

        void RegisterComponent(flecs::world& world) override;
        void RegisterSystem(flecs::world& world) override;
        void RegisterHandler(flecs::world& world) override;

        static void KeyEventHandler(const flecs::world& world, const KeyEvent& event);
    };

}

#endif
