//
// Created by Alex on 04/09/2024.
//

#include "Common/Network/PacketStream.h"

namespace Mcc
{

	PacketInputStream::PacketInputStream(char* begin, size_t size) :
		std::istream(&mBuffer), mBuffer(begin, size)
	{}

	PacketInputStream::PacketInputStream(const ENetPacket* packet) :
		PacketInputStream((char*)(packet->data), packet->dataLength)
	{}

	PacketInputStream::NonOwnedStreamBuf::NonOwnedStreamBuf(char* begin, size_t size)
	{
		setg(begin, begin, begin + size);
	}

	PacketOutputStream::PacketOutputStream() :
		std::ostream(&mBuffer)
	{}

	ByteSpan PacketOutputStream::GetBuffer()
	{
		return { mBuffer.mByteArray };
	}

	PacketOutputStream::OwnedStreamBuf::OwnedStreamBuf()
	{
		setp(&*mByteArray.begin(), &*mByteArray.end());
	}

	std::streamsize PacketOutputStream::OwnedStreamBuf::xsputn(const char* s, std::streamsize n)
	{
		mByteArray.insert(mByteArray.end(), s, s + n);
		setp(&*mByteArray.begin(), &*mByteArray.end());
		return n;
	}

	int PacketOutputStream::OwnedStreamBuf::overflow(int c)
	{
		mByteArray.push_back(static_cast<char>(c));
		return (c);
	}

}