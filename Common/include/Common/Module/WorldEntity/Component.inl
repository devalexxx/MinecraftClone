//
// Created by Alex on 22/06/2025.
//

namespace glm
{

	template<class Archive>
	void serialize(Archive& ar, glm::vec3& vec)
	{
		ar(vec.x, vec.y, vec.z);
	}

	template<class Archive>
	void serialize(Archive& ar, glm::vec2& vec)
	{
		ar(vec.x, vec.y);
	}

}

namespace Mcc
{

	template<class Archive>
	void serialize(Archive& ar, Transform& transform)
	{
		ar(transform.position, transform.rotation, transform.scale);
	}

	template<class Archive>
	void serialize(Archive& ar, Head& head)
	{
		ar(head.pitch);
	}

}