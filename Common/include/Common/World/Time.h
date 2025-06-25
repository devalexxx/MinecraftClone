//
// Created by Alex on 22/09/2024.
//

#ifndef MCC_COMMON_WORLD_TIME_H
#define MCC_COMMON_WORLD_TIME_H

namespace Mcc
{

	using TickRate = unsigned short;

	constexpr TickRate MaxTickRate = 60;
	constexpr TickRate MinTickRate = 1;
	constexpr TickRate DefaultTickRate = 20;

}

#endif
