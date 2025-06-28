//
// Created by Alex on 26/06/2025.
//

#ifndef MCC_CLIENT_MODULE_CAMERA_COMPONENT_H
#define MCC_CLIENT_MODULE_CAMERA_COMPONENT_H

#include <glm/glm.hpp>

namespace Mcc
{

	struct CameraTag {};
	struct CameraFollowTag {};
	struct ActiveCameraTag {};

	struct CameraFollowRelation {};

	struct CameraPrefab {};
	struct CameraFollowPrefab {};

	struct CameraSettings
	{
			float fov;
			float zNear;
			float zFar;
	};

	struct CameraFollowSettings
	{
			glm::vec3 offset;
	};

}

#endif
