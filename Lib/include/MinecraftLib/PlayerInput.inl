
#include "PlayerInput.h"

namespace Mcc
{
	template<typename Archive>
	inline void serialize(Archive& ar, Movement& movement)
	{
		ar(movement.forward, movement.backward, movement.left, movement.right);
	}

	template<typename Archive>
	inline void serialize(Archive& ar, Axis& axis)
	{
		ar(axis.x, axis.y);
	}

	template<typename Archive>
	inline void serialize(Archive& ar, PlayerInputMeta& meta)
	{
		ar(meta.id, meta.dt);
	}

	template<typename Archive>
	inline void serialize(Archive& ar, PlayerInput& input)
	{
		ar(input.meta, input.movement, input.axis);
	}

}