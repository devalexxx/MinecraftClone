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

	void PlayerInput::Apply(Transform& transform, float dt, float speed) const
	{
		if (movement.forward && !movement.backward)
			transform.position += glm::forward(transform.rotation) * speed * dt;

		if (movement.backward && !movement.forward)
			transform.position -= glm::forward(transform.rotation) * speed * dt;

		if (movement.right && !movement.left)
			transform.position += glm::right(transform.rotation) * speed * dt;

		if (movement.left && !movement.right)
			transform.position -= glm::right(transform.rotation) * speed * dt;
	}

	void PlayerInput::Apply(Transform& transform) const
	{
		transform.rotation.x += axis.x;
		transform.rotation.z += axis.y;
	}
}