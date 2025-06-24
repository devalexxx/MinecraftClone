//
// Created by Alex on 23/06/2025.
//

#include <Server/Module/PlayerInput/System.h>

#include <MinecraftLib/Module/WorldEntity/Tag.h>
#include <MinecraftLib/Module/WorldEntity/Component.h>
#include <MinecraftLib/PlayerInput.h>

namespace Mcc
{

	void ProcessPlayerInputs(flecs::entity entity, PlayerInputQueue& queue)
	{
		const float speed = 5.f;

		if (queue.empty())
		{
			return;
		}

		entity.get([&](Position& pos, Rotation& rot, Forward& forward, Right& right, WorldEntityExtra& extra) {
			  for (; !queue.empty(); queue.pop_front())
			  {
				  auto& input = queue.front();
				  input.Apply(rot, forward, right);
				  input.Apply(input.meta.dt, speed, pos, forward, right);

				  if (queue.size() == 1)
				  {
					  extra.data.insert_or_assign("last-input-processed", std::to_string(input.meta.id));
				  }
			  }
		});

		entity.add<WorldEntityUpdatedTag>();
	}

}