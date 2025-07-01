//
// Created by Alex on 29/06/2025.
//

namespace Mcc
{

	template<typename Archive>
	void serialize(Archive& ar, BlockMeta& packet)
	{
		ar(packet.id);
	}

	template<typename Archive>
	void serialize(Archive& ar, ChunkPosition& packet)
	{
		ar(packet.position);
	}

}