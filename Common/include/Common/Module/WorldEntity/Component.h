//
// Created by Alex on 22/06/2025.
//

#ifndef MCC_COMMON_MODULE_WORLD_ENTITY_COMPONENT_H
#define MCC_COMMON_MODULE_WORLD_ENTITY_COMPONENT_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

namespace glm
{

	template<class Archive>
	void serialize(Archive& ar, glm::vec3& vec);

	template<class Archive>
	void serialize(Archive& ar, glm::vec2& vec);

	glm::vec3 forward(const glm::vec3& rotation);
	glm::vec3 right  (const glm::vec3& rotation);

}

namespace Mcc
{

	struct WorldEntityExtra
	{
			std::unordered_map<std::string, std::string> data;
	};

	struct Transform
	{
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
	};

	struct Head
	{
			float pitch;
	};

	template<class Archive>
	void serialize(Archive& ar, Transform& transform);

	template<class Archive>
	void serialize(Archive& ar, Head& head);

}

#include "Common/Module/WorldEntity/Component.inl"

#endif
