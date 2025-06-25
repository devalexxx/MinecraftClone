//
// Created by Alex on 26/08/2024.
//

#ifndef MCC_CHARACTER_H
#define MCC_CHARACTER_H

#include "Common/Entity/Entity.h"

namespace Mcc
{

	class Character : public Entity
	{
		public:
			Character(glm::vec3 headPositionOffset);
			Character(glm::mat4 transform, glm::vec3 headPositionOffset);

			void Rotate(float angle, const glm::vec3& axe) override;

			void LookAt(const glm::vec3& target);

			[[nodiscard]] glm::mat4 GetHeadTransform() const;

		private:
			glm::vec3 mHeadPositionOffset;
			float     mHeadPitch;
	};

}

#endif
