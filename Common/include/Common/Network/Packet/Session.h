//
// Created by Alex Clorennec on 17/07/2025.
//

#ifndef MCC_COMMON_PACKET_SESSION_H
#define MCC_COMMON_PACKET_SESSION_H

#include "Common/Network/NetworkHandle.h"


#include <string>

namespace Mcc
{

    /**
     * Connection flow
     * Client peer initiate connection
     * - Server -> Client => OnWaitingInfo
     * - Client -> Server => OnClientInfo
     * - Server -> Client => OnConnectionAccepted || OnConnectionRefused
     *
     * If client info changed
     * - Client -> Server => OnClientInfoChanged
    **/


    struct ClientInfo
    {
        char tag;
    };

    struct PlayerInfo
    {
        NetworkHandle handle;
    };

    struct ServerInfo
    {
        // Common
        unsigned short tickRate;

        // World related
        unsigned char renderDistance;

        // Player related
        float userSpeed;
    };

    template<typename Archive>
    void serialize(Archive& ar, ClientInfo& packet);
    template<typename Archive>
    void serialize(Archive& ar, PlayerInfo& packet);
    template<typename Archive>
    void serialize(Archive& ar, ServerInfo& packet);

    struct OnWaitingInfo
    {
        char tag;
    };

    struct OnClientInfo
    {
        ClientInfo info;
    };

    struct OnClientInfoChanged
    {
        ClientInfo info;
    };

    struct OnConnectionAccepted
    {
        PlayerInfo playerInfo;
        ServerInfo serverInfo;
    };

    struct OnConnectionRefused
    {
        std::string reason;
    };

    template<typename Archive>
    void serialize(Archive& ar, OnWaitingInfo& packet);
    template<typename Archive>
    void serialize(Archive& ar, OnClientInfo& packet);
    template<typename Archive>
    void serialize(Archive& ar, OnConnectionAccepted& packet);
    template<typename Archive>
    void serialize(Archive& ar, OnConnectionRefused& packet);
    template<typename Archive>
    void serialize(Archive& ar, OnClientInfoChanged& packet);

}

#include "Common/Network/Packet/Session.inl"

#endif
