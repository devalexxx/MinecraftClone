//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_MODULE_CLIENT_PLAYER_ENTITY_MODULE_H
#define MCC_MODULE_CLIENT_PLAYER_ENTITY_MODULE_H

#include <Client/Graphics/Window/Event.h>

#include <MinecraftLib/Module/PlayerEntity/Module.h>
#include <MinecraftLib/Network/Packet.h>

namespace Mcc
{

	class PlayerInputModule
	{
		public:
			PlayerInputModule(flecs::world& world);

		private:
			static void OnEntitiesCreatedHandler(flecs::world& world, const OnEntitiesCreated& event);
			static void OnEntitiesUpdatedHandler(flecs::world& world, const OnEntitiesUpdated& event);

			static void OnKeyEventHandler	   (flecs::world& world, const KeyEvent& 	   event);
			static void OnCursorPosEventHandler(flecs::world& world, const CursorPosEvent& event);
	};

}

#endif
