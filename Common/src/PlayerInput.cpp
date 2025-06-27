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
			transform.position += transform.rotation * glm::forward * speed * dt;

		if (movement.backward && !movement.forward)
			transform.position -= transform.rotation * glm::forward * speed * dt;

		if (movement.right && !movement.left)
			transform.position += transform.rotation * glm::right * speed * dt;

		if (movement.left && !movement.right)
			transform.position -= transform.rotation * glm::right * speed * dt;
	}

	void PlayerInput::Apply(Transform& pTransform, Transform& cTransform) const
	{
		Apply(pTransform);

		glm::vec3 right  = cTransform.rotation * glm::right;
		glm::quat qPitch = glm::angleAxis(axis.y, right);

		glm::quat proposed   = qPitch * cTransform.rotation;
		glm::vec3 newForward = proposed * glm::forward;

		float angle = glm::degrees(glm::asin(glm::normalize(newForward).y));

		if (angle < 80.0f && angle > -80.0f)
		{
			cTransform.rotation = glm::normalize(qPitch * cTransform.rotation);
		}
	}

	void PlayerInput::Apply(Transform& transform) const
	{
		transform.rotation = glm::normalize(glm::angleAxis(axis.x, glm::up) * transform.rotation);
	}
}