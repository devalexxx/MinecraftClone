// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_CLIENT_MODULE_CAMERA_COMPONENT_H
#define MCC_CLIENT_MODULE_CAMERA_COMPONENT_H

#include <glm/glm.hpp>

namespace Mcc
{

    struct CameraTag
    {};
    struct CameraFollowTag
    {};
    struct ActiveCameraTag
    {};

    struct CameraFollowRelation
    {};

    struct CameraPrefab
    {};
    struct CameraFollowPrefab
    {};

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
