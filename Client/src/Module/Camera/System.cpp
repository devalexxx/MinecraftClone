//
// Created by Alex on 28/06/2025.
//

#include "Client/Module/Camera/System.h"
#include "Common/Utils/Logging.h"

namespace Mcc
{

	void CameraFollowSystem(const flecs::iter& it, size_t, Transform& transform, const CameraFollowSettings& settings)
	{
        const auto target = it.pair(3).second();

		target.get([&](const Transform& targetTransform) {
			transform.position = targetTransform.position + settings.offset;
		});
	}

}