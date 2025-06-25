//
// Created by Alex on 23/06/2025.
//

#include "Common/Module/WorldEntity/Component.h"

#include <glm/glm.hpp>

namespace glm
{

	glm::vec3 forward(const glm::vec3& rotation)
	{
		return {
			cos(rotation.z) * sin(rotation.x),
			sin(rotation.z),
			cos(rotation.z) * cos(rotation.x)
		};
	}

	glm::vec3 right(const glm::vec3& rotation)
	{
		return {
			sin(rotation.x - 3.14f / 2.0f),
			0,
			cos(rotation.x - 3.14f / 2.0f)
		};
	}

}