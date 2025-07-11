//
// Created by Alex on 18/06/2025.
//

namespace Mcc
{

    template<IsNetworkManager NManager>
    WorldContext<NManager>* WorldContext<NManager>::Get(const flecs::world& world)
    {
        return static_cast<WorldContext*>(world.get_ctx());
    }

}
