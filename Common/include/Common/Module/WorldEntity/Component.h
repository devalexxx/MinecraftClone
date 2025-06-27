//
// Created by Alex on 22/06/2025.
//

#ifndef MCC_COMMON_MODULE_WORLD_ENTITY_COMPONENT_H
#define MCC_COMMON_MODULE_WORLD_ENTITY_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <unordered_map>
#include <string>
#include <deque>
#include <chrono>

namespace glm
{

	template<class Archive>
	void serialize(Archive& ar, glm::vec3& vec);

	template<class Archive>
	void serialize(Archive& ar, glm::quat& quat);

	static constexpr glm::vec3 forward { 0, 0, -1 };
	static constexpr glm::vec3 right   { 1, 0,  0 };
	static constexpr glm::vec3 up	   { 0, 1,  0 };

//	glm::vec3 forward(const glm::quat& rotation);
//	glm::vec3 right  (const glm::quat& rotation);

}

namespace Mcc
{

	using TimeClock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<TimeClock>;

	struct WorldEntityExtra
	{
			std::unordered_map<std::string, std::string> data;
	};

	struct Transform
	{
			glm::vec3 position;
			glm::quat rotation;
			glm::vec3 scale;
	};

	struct Head
	{
			float pitch;
	};

	struct Snapshot
	{
			Transform transform;
			TimePoint time;
	};

	struct SnapshotQueue
	{
			std::deque<Snapshot> data;
	};

	template<class Archive>
	void serialize(Archive& ar, Transform& transform);

	template<class Archive>
	void serialize(Archive& ar, Head& head);

}

#include "Common/Module/WorldEntity/Component.inl"

#endif
