// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

namespace Mcc
{

    template<IsNetworkManager NManager>
    WorldContext<NManager>* WorldContext<NManager>::Get(const flecs::world& world)
    {
        return static_cast<WorldContext*>(world.get_ctx());
    }

}
