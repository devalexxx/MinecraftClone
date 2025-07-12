//
// Created by Alex Clorennec on 11/07/2025.
//

#include "Server/World/ChunkGenerator.h"

#include "Common/Utils/Logging.h"
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
        for (size_t x = 0; x < Chunk::Size; ++x)
        {
            for (size_t z = 0; z < Chunk::Size; ++z)
            {
                const double noise  = mPerlin.octave2D_01((static_cast<double>(position.x * Chunk::Size + x) * 0.01), (static_cast<double>(position.z * Chunk::Size + z) * 0.01), 4, 1);
                const auto   height = static_cast<size_t>(static_cast<double>(Chunk::Height) * noise);
                for (size_t y = 0; y < height; ++y)
                {
                    chunk.Set({x, y, z}, stone);
                }
            }
        }

        return chunk;
    }

}