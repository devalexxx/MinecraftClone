//
// Created by Alex Clorennec on 11/07/2025.
//

#ifndef MCC_SERVER_WORLD_CHUNK_GENERATOR_H
#define MCC_SERVER_WORLD_CHUNK_GENERATOR_H

#include "Common/World/Chunk.h"

#include "PerlinNoise.hpp"

#include <unordered_map>

namespace Mcc
{

    class ChunkGenerator
    {
        public:
            ChunkGenerator(const flecs::world& world, siv::PerlinNoise::seed_type seed);

            [[nodiscard]] Chunk Generate(glm::ivec3 position) const;

        private:
            std::unordered_map<std::string, flecs::entity> mPalette;
            siv::PerlinNoise mPerlin;
    };

}

#endif
