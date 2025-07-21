//
// Created by Alex on 23/06/2025.
//

#include "Server/Module/Player/System.h"
#include "Server/Module/EntityReplication/Component.h"

#include "Common/Module/Entity/Component.h"
#include "Server/WorldContext.h"

namespace Mcc
{

	void ProcessPlayerInputs(const flecs::entity entity, UserInputQueue& queue)
	{
	    const auto* ctx = ServerWorldContext::Get(entity.world());

		if (queue.data.empty())
		{
			return;
		}

		entity.get([&](Transform& transform, Extra& extra) {
			  for (; !queue.data.empty(); queue.data.pop_front())
			  {
				  auto& input = queue.data.front();
				  Helper::ApplyXAxis   (input, transform);
				  Helper::ApplyMovement(input, transform, ctx->settings.userSpeed, input.meta.dt);

				  if (queue.data.size() == 1)
				  {
					  extra.data.insert_or_assign("last-input-processed", std::to_string(input.meta.id));
				  }
			  }
		});

		entity.add<EntityDirtyTag>();
	}

}