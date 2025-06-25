//
// Created by Alex on 23/08/2024.
//

#include "Common/Entity/Player.h"

namespace Mcc
{

	const glm::vec3 Player::HeadOffset = { 0, 2, 0 };

	Player::Player() :
		Character(HeadOffset)
	{}

	Player::Player(glm::mat4 transform) :
		Character(transform, HeadOffset)
	{}

}