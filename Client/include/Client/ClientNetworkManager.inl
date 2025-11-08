// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

namespace Mcc
{

    template<typename T>
    int ClientNetworkManager::Send(T data, enet_uint32 flag, enet_uint8 channel) const
    {
        return NetworkManager::Send<T>(mPeer, std::move(data), flag, channel);
    }

}