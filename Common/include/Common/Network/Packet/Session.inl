//
// Created by Alex Clorennec on 17/07/2025.
//

#include <cereal/types/string.hpp>

namespace Mcc
{

    template<typename Archive>
    void serialize(Archive& ar, ClientInfo& packet)
    {
        ar(packet.tag);
    }

    template<typename Archive>
    void serialize(Archive& ar, PlayerInfo& packet)
    {
        ar(packet.handle);
    }

    template<typename Archive>
    void serialize(Archive& ar, ServerInfo& packet)
    {
        ar(packet.tickRate, packet.renderDistance, packet.userSpeed);
    }

    template<typename Archive>
    void serialize(Archive& ar, OnWaitingInfo& packet)
    {
        ar(packet.tag);
    }

    template<typename Archive>
    void serialize(Archive& ar, OnClientInfo& packet)
    {
        ar(packet.info);
    }

    template<typename Archive>
    void serialize(Archive& ar, OnConnectionAccepted& packet)
    {
        ar(packet.playerInfo, packet.serverInfo);
    }

    template<typename Archive>
    void serialize(Archive& ar, OnConnectionRefused& packet)
    {
        ar(packet.reason);
    }

    template<typename Archive>
    void serialize(Archive& ar, OnClientInfoChanged& packet)
    {
        ar(packet.info);
    }

}
