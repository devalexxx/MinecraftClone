//
// Created by Alex on 17/06/2025.
//

#include "Common/PlayerInput.h"
#include "Common/Module/WorldEntity/Component.h"

namespace Mcc
{

	unsigned short PlayerInputMeta::GetNextID()
	{
		static unsigned short next = 0;
		return next++;
	}

	bool PlayerInput::NotNull() const
	{
		return (
			movement.forward || movement.backward || movement.left || movement.right || axis.x != 0 || axis.y != 0
		);
	}

	void PlayerInput::Apply(float dt, float speed, Position& position, const Forward& forward, const Right& right) const
	{
		if (movement.forward && !movement.backward)
			position.vec += forward.vec * speed * dt;

		if (movement.backward && !movement.forward)
			position.vec -= forward.vec * speed * dt;

		if (movement.right && !movement.left)
			position.vec += right.vec * speed * dt;

		if (movement.left && !movement.right)
			position.vec -= right.vec * speed * dt;
	}

	void PlayerInput::Apply(Rotation& rotation, Forward& forward, Right& right) const
	{
		rotation.vec.x += axis.x;
		rotation.vec.y += axis.y;

		forward.vec = glm::forward(rotation.vec);
		right.vec 	= glm::right  (rotation.vec);
	}
}