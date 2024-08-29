//
// Created by Alex on 26/08/2024.
//

#include <MinecraftLib/Entity/Entity.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Mcc
{

	Entity::Entity() :
		Entity(glm::mat4(1.f))
	{}

	Entity::Entity(glm::mat4 transform) :
		mTransform(transform)
	{}

	void Entity::Translate(const glm::vec3& position)
	{
		mTransform = glm::translate(mTransform, position);
		Dispatch<EntityTransformUpdateEvent>({});
	}

	void Entity::Rotate(float angle, const glm::vec3& axe)
	{
		mTransform = glm::rotate(glm::mat4(1.f), angle, axe) * mTransform;
		Dispatch<EntityTransformUpdateEvent>({});
	}

	void Entity::Scale(const glm::vec3& scale)
	{
		mTransform = glm::scale(mTransform, scale);
		Dispatch<EntityTransformUpdateEvent>({});
	}

	const glm::mat4& Entity::GetTransform() const
	{
		return mTransform;
	}

	glm::vec3 Entity::GetPosition() const
	{
		return mTransform[3];
	}

	glm::quat Entity::GetRotation() const
	{
		auto scale = GetScale();

		const glm::mat3 rotationMatrix(
			glm::vec3(mTransform[0]) / scale[0],
			glm::vec3(mTransform[1]) / scale[1],
			glm::vec3(mTransform[2]) / scale[2]
		);

		return glm::quat_cast(rotationMatrix);
	}

	glm::vec3 Entity::GetScale() const
	{
		glm::vec3 scale;
		for (int i = 0; i < 3; ++i)
		{
			scale[i] = glm::length(glm::vec3(mTransform[i]));
		}
		return scale;
	}


}