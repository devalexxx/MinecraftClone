//
// Created by Alex on 03/09/2024.
//

#ifndef MINECRAFT_TRANSFORM_H
#define MINECRAFT_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

#include <cereal/cereal.hpp>

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
