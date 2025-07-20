//
// Created by Alex Clorennec on 11/07/2025.
//

#include "Server/World/ChunkGenerator.h"

#include "Common/Utils/Logging.h"

#include <utility>

namespace Mcc
{

    ChunkGenerator::ChunkGenerator(const flecs::world& world, const siv::PerlinNoise::seed_type seed) : mPerlin(seed)
    {
        mPalette.emplace("air",   world.lookup("mcc:block:air"));
        mPalette.emplace("stone", world.lookup("mcc:block:stone"));
        mPalette.emplace("dirt",  world.lookup("mcc:block:dirt"));
    }

    Chunk ChunkGenerator::Generate(const glm::ivec3 position) const
    {
        Chunk chunk(mPalette.find("air")->second);
        const auto stone = mPalette.find("stone")->second;
        for (int x = 0; std::cmp_less(x , Chunk::Size); ++x)
        {
            for (int z = 0; std::cmp_less(z , Chunk::Size); ++z)
            {
                constexpr auto f    = .01;
                constexpr auto oct  = 2;
                constexpr auto max  = .9;
                constexpr auto min  = .0;
                const     auto dx   = static_cast<double>(position.x * Chunk::Size + x);
                const     auto dy   = static_cast<double>(position.z * Chunk::Size + z);
                const double noise  = mPerlin.octave2D_01(dx * f, dy * f, oct);
                const double mapped = (max - min) * (noise - 0.) / (1. + 0.) + min;
                const auto   height = static_cast<size_t>(static_cast<double>(Chunk::Height) * (mapped + (1. - max)));
                for (size_t y = 0; y < height; ++y)
                {
                    chunk.Set({x, y, z}, stone);
                }
            }
        }

        return chunk;
    }

}