//
// Created by Alex on 26/08/2024.
//

#ifndef MINECRAFT_ENTITY_H
#define MINECRAFT_ENTITY_H

#include <MinecraftLib/Utils/EventManager.h>

#include <functional>
#include <glm/glm.hpp>

namespace Mcc
{

	struct EntityEventTag
	{};

	struct EntityTransformUpdateEvent
	{};

	class Entity: public EventManager<EntityEventTag>
	{
		public:
			Entity();
			Entity(glm::mat4 transform);

			virtual void Translate(const glm::vec3& position);
			virtual void Rotate(float angle, const glm::vec3& axe);
			virtual void Scale(const glm::vec3& scale);

			[[nodiscard]] const glm::mat4& GetTransform() const;

			[[nodiscard]] glm::vec3 GetPosition() const;
			[[nodiscard]] glm::quat GetRotation() const;
			[[nodiscard]] glm::vec3 GetScale() const;

		private:
			glm::mat4 mTransform;
	};

}

#endif
