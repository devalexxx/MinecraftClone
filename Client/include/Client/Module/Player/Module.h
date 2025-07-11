//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_PLAYER_MODULE_H
#define MCC_CLIENT_MODULE_PLAYER_MODULE_H

#include "Client/Graphics/Window/Event.h"

#include "Common/Network/Packet.h"

#include <flecs.h>

namespace Mcc
{

	class PlayerModule
	{
		public:
			PlayerModule(flecs::world& world);

		private:

			static void OnEntitiesCreatedHandler(const flecs::world& world, const OnEntitiesCreated& event);
			static void OnEntitiesUpdatedHandler(const flecs::world& world, const OnEntitiesUpdated& event);

			static void OnKeyEventHandler	    (const flecs::world& world, const KeyEvent& 	   	   event);
			static void OnCursorPosEventHandler (const flecs::world& world, const CursorPosEvent&    event);

	};

}

#endif
