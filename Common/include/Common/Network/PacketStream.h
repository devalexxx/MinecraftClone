// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef MCC_COMMON_NETWORK_PACKET_STREAM_H
#define MCC_COMMON_NETWORK_PACKET_STREAM_H

#include "Common/Export.h"

#include <enet/enet.h>

#include <iostream>
#include <span>
#include <vector>

namespace Mcc
{

    using Byte      = char;
    using ByteSpan  = std::span<Byte>;
    using ByteArray = std::vector<Byte>;

    class MCC_LIB_API PacketInputStream final : public std::istream
    {
      public:
        explicit PacketInputStream(const ENetPacket* packet);
        PacketInputStream(char* begin, size_t size);

      private:
        struct NonOwnedStreamBuf final : std::streambuf
        {
            NonOwnedStreamBuf(char* begin, size_t size);
        } mBuffer;
    };

    class MCC_LIB_API PacketOutputStream final : public std::ostream
    {
      public:
        PacketOutputStream();

        ByteSpan GetBuffer();

      private:
        struct OwnedStreamBuf final : std::streambuf
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