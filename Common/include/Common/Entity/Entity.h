//
// Created by Alex on 26/08/2024.
//

#ifndef MCC_ENTITY_H
#define MCC_ENTITY_H

#include "Common/Utils/EventManager.h"

#include <glm/glm.hpp>

#include <functional>

namespace Mcc
{

	struct EntityEventTag
	{};

	struct EntityTransformUpdateEvent
	{};

	class Entity : public EventManager<EntityEventTag>
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
