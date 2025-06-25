//
// Created by Alex on 13/09/2024.
//

#ifndef MCC_COMMON_PLAYER_INPUT_H
#define MCC_COMMON_PLAYER_INPUT_H

namespace Mcc
{
	struct Position;
	struct Rotation;
	struct Forward;
	struct Right;

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

	struct PlayerInputMeta
	{
			unsigned short id;
			float 		   dt;

			static unsigned short GetNextID();
	};

	struct PlayerInput
	{
			PlayerInputMeta meta;
			Movement 		movement;
			Axis	 		axis;

			[[nodiscard]] bool NotNull() const;

			void Apply(float dt, float speed, Position& position, const Forward& forward, const Right& right) const;
			void Apply(Rotation& rotation, Forward& forward, Right& right) const;
	};

	template<typename Archive>
	inline void serialize(Archive& ar, Movement& movement);
	template<typename Archive>
	inline void serialize(Archive& ar, Axis& axis);
	template<typename Archive>
	inline void serialize(Archive& ar, PlayerInputMeta& meta);
	template<typename Archive>
	inline void serialize(Archive& ar, PlayerInput& input);

}

#include <Common/PlayerInput.inl>

#endif
