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

		if (queue.data.empty())
		{
			return;
		}

		entity.get([&](Transform& transform, WorldEntityExtra& extra) {
			  for (; !queue.data.empty(); queue.data.pop_front())
			  {
				  auto& input = queue.data.front();
				  input.Apply(transform);
				  input.Apply(transform, input.meta.dt, speed);

				  if (queue.data.size() == 1)
				  {
					  extra.data.insert_or_assign("last-input-processed", std::to_string(input.meta.id));
				  }
			  }
		});

		entity.add<WorldEntityUpdatedTag>();
	}

}