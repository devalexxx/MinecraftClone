//
// Created by Alex on 26/08/2024.
//

#include <MinecraftLib/Entity/Character.h>
#include <fmt/base.h>
#include <glm/ext/matrix_transform.hpp>

namespace Mcc
{
	Character::Character(glm::vec3 headPositionOffset) :
		Character(glm::mat4(1.f), headPositionOffset)
	{}

	Character::Character(glm::mat4 transform, glm::vec3 headPositionOffset) :
		Entity(transform),
		mHeadPositionOffset(headPositionOffset),
		mHeadPitch(0.f)
	{}

	void Character::Rotate(float angle, const glm::vec3& axe)
	{
		static auto up   = glm::vec3(1, 0, 0);
		static auto down = glm::vec3(-1, 0, 0);

		if (axe == up || axe == down)
		{
			mHeadPitch = glm::clamp(mHeadPitch + axe.x * angle, glm::radians(-45.f), glm::radians(45.f));
			Dispatch<EntityTransformUpdateEvent>({});
		}
		else
		{
			Entity::Rotate(angle, axe);
		}
	}

	void Character::LookAt(const glm::vec3& target)
	{
	}

	glm::mat4 Character::GetHeadTransform() const
	{
		return glm::rotate(glm::mat4(1.f), mHeadPitch, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.f), -mHeadPositionOffset) * GetTransform();
	}

}