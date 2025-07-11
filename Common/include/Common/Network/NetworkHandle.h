//
// Created by Alex on 18/06/2025.
//

#ifndef MCC_COMMON_NETWORK_NETWORK_HANDLE_H
#define MCC_COMMON_NETWORK_NETWORK_HANDLE_H

namespace Mcc
{
	using NetworkHandle = unsigned int;

    inline NetworkHandle Null()
    {
        return 0;
    }

	inline NetworkHandle GenerateNetworkHandle()
	{
		static NetworkHandle current = 0;
		return ++current;
	}

	inline bool IsValid(const NetworkHandle handle)
	{
		return handle > 0;
	}


}

#endif