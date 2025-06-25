//
// Created by Alex on 23/06/2025.
//

#ifndef MCC_SERVER_MODULE_PLAYER_SESSION_MODULE_H
#define MCC_SERVER_MODULE_PLAYER_SESSION_MODULE_H

#include "Common/Module/WorldEntity/Component.h"
#include "Common/Network/Event.h"

#include <flecs.h>

namespace Mcc
{

	class PlayerSessionModule
	{
		public:
			PlayerSessionModule(flecs::world& world);

		private:
			flecs::query<const Position, const Rotation> mLookupQuery;

			void OnConnectEventHandler   (flecs::world& world, const ConnectEvent&    event) const;
			void OnDisconnectEventHandler(flecs::world& world, const DisconnectEvent& event) const;
	};

}

#endif
