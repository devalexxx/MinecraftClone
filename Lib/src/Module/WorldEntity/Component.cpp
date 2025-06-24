//
// Created by Alex on 23/06/2025.
//

#include <MinecraftLib/Module/WorldEntity/Component.h>

#include <glm/glm.hpp>

namespace glm
{

	glm::vec3 forward(const glm::vec2& rotation)
	{
		return {
			cos(rotation.y) * sin(rotation.x),
			sin(rotation.y),
			cos(rotation.y) * cos(rotation.x)
		};
	}

	glm::vec3 right(const glm::vec2& rotation)
	{
		return {
			sin(rotation.x - 3.14f / 2.0f),
			0,
			cos(rotation.x - 3.14f / 2.0f)
		};
	}

}

namespace Mcc
{

	Forward Forward::FromRotation(const Rotation& rotation)
	{
		return { glm::forward(rotation.vec) };
	}

	Right Right::FromRotation(const Rotation& rotation)
	{
		return { glm::right(rotation.vec) };
	}

}