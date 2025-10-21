//
// Created by Alex on 18/06/2025.
//

#ifndef MCC_COMMON_WORLD_CONTEXT_H
#define MCC_COMMON_WORLD_CONTEXT_H

#include "Common/Network/NetworkHandle.h"
#include "Common/Network/NetworkManager.h"
#include "Common/Network/Packet.h"

#include "Hexis/Core/ThreadPool.h"

#include <unordered_map>

namespace Mcc
{

    struct NetworkMapping
    {
        using LHandle = flecs::entity_t;
        using RHandle = NetworkHandle;

        std::unordered_map<LHandle, RHandle> localToRemote{};
        std::unordered_map<RHandle, LHandle> remoteToLocal{};

        void Set(RHandle rHandle, LHandle lHandle);

        void RRemove(RHandle handle);
        void LRemove(LHandle handle);

        [[nodiscard]] std::optional<LHandle> GetLHandle(RHandle handle) const;
        [[nodiscard]] std::optional<RHandle> GetRHandle(LHandle handle) const;
    };

    template<typename T>
    concept IsNetworkManager = std::is_base_of_v<NetworkManager, T> || std::is_same_v<NetworkManager, T>;

    template<IsNetworkManager NManager = NetworkManager>
	struct WorldContext
	{
            static WorldContext* Get(const flecs::world& world);

			NManager&       networkManager;
            NetworkMapping  networkMapping;
            Hx::ThreadPool& threadPool;
	};

}

#include "Common/WorldContext.inl"

#endif
