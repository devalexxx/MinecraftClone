//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_CLIENT_MODULE_CAMERA_SYSTEM_H
#define MCC_CLIENT_MODULE_CAMERA_SYSTEM_H

#include "Client/Module/Camera/Component.h"

#include "Common/Module/Entity/Component.h"

#include <flecs.h>

namespace Mcc
{

	void CameraFollowSystem(const flecs::iter& it, size_t, Transform& transform, const CameraFollowSettings& settings);

}

#endif
