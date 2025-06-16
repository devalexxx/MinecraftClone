//
// Created by Alex on 04/09/2024.
//

#ifndef MINECRAFT_PACKETSTREAM_H
#define MINECRAFT_PACKETSTREAM_H

#include <streambuf>
#include <iostream>
#include <vector>
#include <optional>
#include <span>

#include <enet/enet.h>

namespace Mcc
{

	using Byte 		= char;
	using ByteSpan  = std::span<Byte>;
	using ByteArray = std::vector<Byte>;

	class PacketInputStream : public std::istream
	{
		public:
			PacketInputStream(char* begin, size_t size);
			PacketInputStream(const ENetPacket* packet);

		private:
			struct NonOwnedStreamBuf : std::streambuf
			{
				NonOwnedStreamBuf(char* begin, size_t size);
			} mBuffer;
	};

	class PacketOutputStream : public std::ostream
	{
		public:
			PacketOutputStream();

			ByteSpan GetBuffer();

		private:
			struct OwnedStreamBuf : std::streambuf
			{
				OwnedStreamBuf();

				ByteArray mByteArray;

				protected:
					std::streamsize xsputn(const char* s, std::streamsize n) override;
					int overflow(int c) override;
			} mBuffer;
	};

}

#endif
