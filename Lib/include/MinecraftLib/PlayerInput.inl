
namespace Mcc
{

	template<typename Archive>
	inline void serialize(Archive& ar, Movement& movement)
	{
		ar(movement.forward, movement.backward, movement.left, movement.right);
	}

	template<typename Archive>
	inline void serialize(Archive& ar, Axis& axis)
	{
		ar(axis.x, axis.y);
	}

	template<typename Archive>
	inline void serialize(Archive& ar, PlayerInput& input)
	{
		ar(input.movement, input.axis);
	}

}