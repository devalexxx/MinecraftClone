//
// Created by Alex on 23/08/2024.
//

#ifndef MINECRAFT_PLAYER_H
#define MINECRAFT_PLAYER_H

#include <MinecraftLib/Entity/Character.h>

#include <functional>
#include <glm/glm.hpp>

namespace Mcc
{

	class Player : public Character
	{
		public:
			static const glm::vec3 HeadOffset;

		public:
			Player();
			Player(glm::mat4 transform);
	};

}

#endif
