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
	void serialize(Archive& ar, glm::vec3& vec)
	{
		ar(vec.x, vec.y, vec.z);
	}

	template<class Archive>
	void serialize(Archive& ar, glm::vec2& vec)
	{
		ar(vec.x, vec.y);
	}

	glm::vec3 forward(const glm::vec2& rotation);
	glm::vec3 right  (const glm::vec2& rotation);

}

namespace Mcc
{

	struct WorldEntityExtra
	{
			std::unordered_map<std::string, std::string> data;
	};

	struct Position
	{
			glm::vec3 vec;
	};

	struct Rotation
	{
			glm::vec2 vec;
	};

	struct Forward
	{
			glm::vec3 vec;

			static Forward FromRotation(const Rotation& rotation);
	};

	struct Right
	{
			glm::vec3 vec;

			static Right FromRotation(const Rotation& rotation);
	};

	struct Head
	{
			float pitch;
	};

	template<class Archive>
	void serialize(Archive& ar, Position& position)
	{
		ar(position.vec);
	}

	template<class Archive>
	void serialize(Archive& ar, Rotation& rotation)
	{
		ar(rotation.vec);
	}

	template<class Archive>
	void serialize(Archive& ar, Forward& direction)
	{
		ar(direction.vec);
	}

	template<class Archive>
	void serialize(Archive& ar, Right& right)
	{
		ar(right.vec);
	}

	template<class Archive>
	void serialize(Archive& ar, Head& head)
	{
		ar(head.pitch);
	}

}

#endif
