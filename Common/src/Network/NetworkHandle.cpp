//
// Created by Alex Clorennec on 09/11/2025.
//

#include "Common/Network/NetworkHandle.h"

namespace Mcc
{

    NetworkHandle GenerateNetworkHandle()
    {
        static NetworkHandle current = 0;
        return ++current;
    }

}