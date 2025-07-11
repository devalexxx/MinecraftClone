//
// Created by Alex Clorennec on 10/07/2025.
//

#include "Common/WorldContext.h"

namespace Mcc
{

    void NetworkMapping::Set(RHandle rHandle, LHandle lHandle)
    {
        remoteToLocal.emplace(rHandle, lHandle);
        localToRemote.emplace(lHandle, rHandle);
    }

    void NetworkMapping::RRemove(const RHandle handle)
    {
        if (const auto other = GetLHandle(handle); other.has_value())
        {
            remoteToLocal.erase(handle);
            localToRemote.erase(*other);
        }
    }

    void NetworkMapping::LRemove(const LHandle handle)
    {
        if (const auto other = GetRHandle(handle); other.has_value())
        {
            localToRemote.erase(handle);
            remoteToLocal.erase(*other);
        }
    }

    auto NetworkMapping::GetLHandle(const RHandle handle) const -> std::optional<LHandle>
    {
        if (const auto it = remoteToLocal.find(handle); it != remoteToLocal.end())
        {
            return { it->second };
        }

        return std::nullopt;
    }

    auto NetworkMapping::GetRHandle(const LHandle handle) const -> std::optional<RHandle>
    {
        if (const auto it = localToRemote.find(handle); it != localToRemote.end())
        {
            return { it->second };
        }

        return std::nullopt;
    }


}
