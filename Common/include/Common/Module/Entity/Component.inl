//
// Created by Alex on 28/06/2025.
//

namespace glm
{

	template<class Archive>
	void serialize(Archive& ar, glm::vec3& vec)
	{
		ar(vec.x, vec.y, vec.z);
	}

	template<class Archive>
	void serialize(Archive& ar, glm::uvec3& vec)
	{
		ar(vec.x, vec.y, vec.z);
	}

	template<class Archive>
	void serialize(Archive& ar, glm::quat& quat)
	{
		ar(quat.x, quat.y, quat.z, quat.w);
	}

}

namespace Mcc
{

	template<class Archive>
	void serialize(Archive& ar, Transform& transform)
	{
		ar(transform.position, transform.rotation, transform.scale);
	}

	template<typename Archive>
	void serialize(Archive& ar, UserInput& input)
	{
		ar(
			input.movement.forward, input.movement.backward, input.movement.right, input.movement.left,
			input.movement.up, input.movement.down,
			input.axis.x, input.axis.y,
			input.meta.id, input.meta.dt
		);
	}

}
