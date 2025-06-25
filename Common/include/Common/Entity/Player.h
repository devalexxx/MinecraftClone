//
// Created by Alex on 23/08/2024.
//

#ifndef MCC_PLAYER_H
#define MCC_PLAYER_H

#include "Common/Entity/Character.h"

#include <glm/glm.hpp>

#include <functional>

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
