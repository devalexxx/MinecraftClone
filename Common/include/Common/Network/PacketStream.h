// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_NETWORK_PACKET_STREAM_H
#define MCC_COMMON_NETWORK_PACKET_STREAM_H

#include <enet/enet.h>

#include <iostream>
#include <span>
#include <vector>

namespace Mcc
{

    using Byte      = char;
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
            int             overflow(int c) override;
        } mBuffer;
    };

}

#endif