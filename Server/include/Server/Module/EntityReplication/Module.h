// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_SERVER_MODULE_ENTITY_REPLICATION_MODULE_H
#define MCC_SERVER_MODULE_ENTITY_REPLICATION_MODULE_H

#include <flecs.h>

namespace Mcc
{

    struct EntityReplicationModule
    {
        explicit EntityReplicationModule(const flecs::world& world);
    };

}

#endif
