//
// Created by Alex on 13/09/2024.
//

#ifndef MINECRAFT_PLAYERINPUT_H
#define MINECRAFT_PLAYERINPUT_H

#include <queue>

namespace Mcc
{
	struct Movement
	{
			bool forward;
			bool backward;
			bool left;
			bool right;
	};

	struct Axis
	{
			float x;
			float y;
	};

	struct PlayerInput
	{
		Movement movement;
		Axis	 axis;
	};

	template<typename Archive>
	inline void serialize(Archive& ar, Movement& movement);
	template<typename Archive>
	inline void serialize(Archive& ar, Axis& axis);
	template<typename Archive>
	inline void serialize(Archive& ar, PlayerInput& input);

}

#include <MinecraftLib/PlayerInput.inl>

#endif
