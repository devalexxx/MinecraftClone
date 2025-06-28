//
// Created by Alex on 28/06/2025.
//

#ifndef MCC_COMMON_MODULE_ENTITY_COMPONENT_H
#define MCC_COMMON_MODULE_ENTITY_COMPONENT_H

#include "Common/Network/NetworkID.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <unordered_map>
#include <deque>
#include <string>

namespace glm
{

	template<class Archive>
	void serialize(Archive& ar, glm::vec3& vec);

	template<class Archive>
	void serialize(Archive& ar, glm::quat& quat);

	static constexpr glm::vec3 forward { 0, 0, -1 };
	static constexpr glm::vec3 right   { 1, 0, 0 };
	static constexpr glm::vec3 up 	   { 0, 1, 0 };

}

namespace Mcc
{

	struct EntityTag {};
	struct NetworkEntityTag {};
	struct UserEntityTag {};

	struct EntityPrefab {};
	struct NetworkEntityPrefab {};
	struct UserEntityPrefab {};

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

	struct NetworkHandle
	{
			NetworkID id;
	};

	struct UserInput
	{
			struct Meta
			{
					unsigned short id;
					float 		   dt;

					static unsigned short GetNextID();
			};

			struct
			{
					bool forward;
					bool backward;
					bool left;
					bool right;
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

		bool IsNull(const UserInput& input);
		void ApplyMovement(const UserInput& input, Transform& transform, float speed, float dt);
		void ApplyXAxis	  (const UserInput& input, Transform& transform);
		void ApplyYAxis	  (const UserInput& input, Transform& transform);
		void ApplyBothAxis(const UserInput& input, Transform& transform);

	}

}

#include "Common/Module/Entity/Component.inl"

#endif
