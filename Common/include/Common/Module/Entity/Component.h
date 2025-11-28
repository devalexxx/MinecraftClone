// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_MODULE_ENTITY_COMPONENT_H
#define MCC_COMMON_MODULE_ENTITY_COMPONENT_H

#include "Common/Export.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <deque>
#include <string>
#include <unordered_map>

namespace glm
{

    template<class Archive>
    void serialize(Archive& ar, vec3& vec);

    template<class Archive>
    void serialize(Archive& ar, uvec3& vec);

    template<class Archive>
    void serialize(Archive& ar, ivec3& vec);

    template<class Archive>
    void serialize(Archive& ar, quat& quat);

    static constexpr vec3 forward { 0, 0, -1 };
    static constexpr vec3 back { 0, 0, 1 };
    static constexpr vec3 right { 1, 0, 0 };
    static constexpr vec3 left { -1, 0, 0 };
    static constexpr vec3 up { 0, 1, 0 };
    static constexpr vec3 down { 0, -1, 0 };

}

namespace Mcc
{

    struct EntityTag
    {};
    struct NetworkEntityTag
    {};
    struct UserEntityTag
    {};

    struct EntityPrefab
    {};
    struct NetworkEntityPrefab
    {};
    struct UserEntityPrefab
    {};

    struct Transform
    {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    };

    struct Extra
    {
        std::unordered_map<std::string, std::string> data;
    };

    struct MCC_LIB_API UserInput
    {
        struct Meta
        {
            unsigned short id;
            float          dt;

            static unsigned short GetNextID();
        };

        struct
        {
            bool forward;
            bool backward;
            bool left;
            bool right;
            bool up;
            bool down;
        } movement;

        struct
        {
            float x;
            float y;
        } axis;

        Meta meta;
    };

    struct UserInputQueue
    {
        std::deque<UserInput> data;
    };

    template<class Archive>
    void serialize(Archive& ar, Transform& transform);
    template<typename Archive>
    void serialize(Archive& ar, UserInput& input);

    namespace Helper
    {

        MCC_LIB_API bool IsNull(const UserInput& input);
        MCC_LIB_API void ApplyMovement(const UserInput& input, Transform& transform, float speed, float dt);
        MCC_LIB_API void ApplyXAxis(const UserInput& input, Transform& transform);
        MCC_LIB_API void ApplyYAxis(const UserInput& input, Transform& transform);
        MCC_LIB_API void ApplyBothAxis(const UserInput& input, Transform& transform);

    }

}

#include "Common/Module/Entity/Component.inl"

#endif
