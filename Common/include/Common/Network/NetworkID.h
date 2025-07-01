//
// Created by Alex on 18/06/2025.
//

#ifndef MCC_COMMON_NETWORK_NETWORK_ID_H
#define MCC_COMMON_NETWORK_NETWORK_ID_H

namespace Mcc
{
	using NetworkID = unsigned int;

	inline NetworkID GenerateNetworkID()
	{
		static NetworkID current = 0;
		return ++current;
	}

	inline bool IsValid(NetworkID id)
	{
		return id > 0;
	}


}

#endif