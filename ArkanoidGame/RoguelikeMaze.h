#pragma once
#include <vector>
#include <cstdint>
#include <random>
#include "../Engine/Vector.h"

namespace RogaliqueGame
{
    class RoguelikeMaze
    {
    public:
        static constexpr int CELLS_PER_BLOCK_X = 20;
        static constexpr int CELLS_PER_BLOCK_Y = 12;
        static constexpr int GLOBAL_CELLS_X = 40;
        static constexpr int GLOBAL_CELLS_Y = 24;
        static constexpr float CELL_W = 64.f;
        static constexpr float CELL_H = 60.f;
        static constexpr int BLOCK_COUNT = 4;
        static constexpr float BLOCK_W = 1280.f;
        static constexpr float BLOCK_H = 720.f;
        static constexpr float WORLD_W = 2560.f;
        static constexpr float WORLD_H = 1440.f;

        void Generate(uint32_t seed);
        uint32_t GetSeed() const { return seed; }

        bool IsWall(int gx, int gy) const;
        bool IsWallWorld(float x, float y) const;
        bool IsWalkableWorld(float x, float y, float radius) const;

        int BlockAt(float x, float y) const;
        ::Engine::Vector2Df BlockOrigin(int block) const;
        ::Engine::Vector2Df BlockCenterWorld(int block) const;

        ::Engine::Vector2Df GlobalCellCenter(int gx, int gy) const;
        bool WorldToGlobal(float x, float y, int& gx, int& gy) const;

        std::vector<::Engine::Vector2Df> FindPath(::Engine::Vector2Df fromWorld, ::Engine::Vector2Df toWorld) const;

        ::Engine::Vector2Df RandomRoomCenter(int block = -1) const;

    private:
        bool walls[GLOBAL_CELLS_X][GLOBAL_CELLS_Y] = {};
        uint32_t seed = 0;

        void GenerateBlock(int block, std::mt19937& rng);
        void CarveConnections(std::mt19937& rng);
        ::Engine::Vector2Df LocalRoomCenter(int block, int localX, int localY) const;
    };
}