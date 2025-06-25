//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/PlayerInput/System.h"

#include "Common/Module/WorldEntity/Tag.h"
#include "Common/Module/WorldEntity/Component.h"
#include "Common/PlayerInput.h"

namespace Mcc
{

	void ProcessPlayerInputs(flecs::entity entity, PlayerInputQueue& queue)
	{
		const float speed = 5.f;

		if (queue.empty())
		{
			return;
		}

		entity.get([&](Transform& transform, WorldEntityExtra& extra) {
			  for (; !queue.empty(); queue.pop_front())
			  {
				  auto& input = queue.front();
				  input.Apply(transform);
				  input.Apply(transform, input.meta.dt, speed);

				  if (queue.size() == 1)
				  {
					  extra.data.insert_or_assign("last-input-processed", std::to_string(input.meta.id));
				  }
			  }
		});

		entity.add<WorldEntityUpdatedTag>();
	}

}