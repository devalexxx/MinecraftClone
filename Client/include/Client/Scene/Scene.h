// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_SCENE_SCENE_H
#define MCC_CLIENT_SCENE_SCENE_H

#include "Client/Scene/GameScene/Scene.h"
#include "Client/Scene/MenuScene/Scene.h"

#include "Common/SceneImporter.h"

namespace Mcc
{

    struct CSceneImporter;

    struct MenuScene : Scene<MenuScene, CSceneImporter>
    {};
    struct GameScene : Scene<GameScene, CSceneImporter>
    {};

    using Importer = SceneImporter<MenuScene, GameScene>;

    struct CSceneImporter : Importer
    {
        using Importer::SceneImporter;
    };

}

#endif
