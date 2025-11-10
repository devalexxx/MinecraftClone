// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "Common/Network/Event.h"

namespace Mcc
{

    template<typename T>
    void NetworkEventManager::HandlerMapper::operator()() const
    {
        constexpr size_t index = PacketList::IndexOf<T>;

        mEventManager.mHandlers[index] = [](ENetPeer* peer, const ENetPacket* packet, NetworkEventManager& eventManager,
                                            cereal::BinaryInputArchive&& archive) {
            T data {};
            try
            {
                archive(data);
            }
            catch (std::exception& exc)
            {
                eventManager.Dispatch<MalformedPacketEvent>({ packet });
                return;
            }

            eventManager.Dispatch<T>(data);
            eventManager.Dispatch<From<T>>({ peer, std::move(data) });
        };
    }

}