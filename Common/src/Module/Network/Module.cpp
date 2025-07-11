//
// Created by Alex Clorennec on 10/07/2025.
//

#include "Common/Module/Network/Module.h"
#include "Common/Module/Network/System.h"
#include "Common/Module/Network/Component.h"
#include "Common/Utils/Logging.h"
#include "Common/WorldContext.h"

namespace Mcc
{

     NetworkModule::NetworkModule(const flecs::world& world)
    {
         MCC_LOG_DEBUG("Import NetworkModule...");
         world.module<NetworkModule>();

         world.component<ServerTag>();
         world.component<ClientTag>();
         world.component<NetworkObjectTag>();

         world.component<NetworkProps>();

         world.prefab<NetworkObjectPrefab>()
            .add<NetworkObjectTag>()
            .set_auto_override<NetworkProps>({ Null() });

         world.observer<NetworkProps>()
            .event(flecs::OnAdd)
            .with<ServerTag>().singleton()
            .each(GenerateNetworkHandleObserver);

         world.observer<const NetworkProps>()
            .event(flecs::OnSet)
            .run(AddToContextObserver);

         world.observer<const NetworkProps>()
            .event(flecs::OnRemove)
            .run(RemoveFromContextObserver);
    }


}