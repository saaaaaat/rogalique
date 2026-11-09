#include "pch.h"
#include "RoguelikeMaze.h"
#include "Logger.h"
#include <random>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <Vector.h>
#include <stack>

using namespace Engine;

namespace RogaliqueGame
{
    void RoguelikeMaze::Generate(uint32_t newSeed)
    {
        seed = newSeed;
        std::mt19937 rng(seed);

        for (int x = 0; x < GLOBAL_CELLS_X; x++)
            for (int y = 0; y < GLOBAL_CELLS_Y; y++)
                walls[x][y] = true;

        for (int b = 0; b < BLOCK_COUNT; b++)
            GenerateBlock(b, rng);

        CarveConnections(rng);

        LOG_INFO("Maze generated, seed=" + std::to_string(seed));
    }


    void RoguelikeMaze::GenerateBlock(int block, std::mt19937& rng)
    {
        int bc = block % 2, br = block / 2;
        int offX = bc * CELLS_PER_BLOCK_X;
        int offY = br * CELLS_PER_BLOCK_Y;

        std::stack<std::pair<int, int>> stack;


        int startX = 1;
        int startY = 1;

        stack.push({ startX, startY });
        walls[offX + startX][offY + startY] = false;

        const std::vector<std::pair<int, int>> directions = {
            {0, -2}, {0, 2}, {-2, 0}, {2, 0}
        };

        while (!stack.empty())
        {
            auto [cx, cy] = stack.top();

           
            std::vector<std::pair<int, int>> available;
            for (const auto& dir : directions)
            {
                int nx = cx + dir.first;
                int ny = cy + dir.second;

                if (nx > 0 && nx < CELLS_PER_BLOCK_X && ny > 0 && ny < CELLS_PER_BLOCK_Y
                    && walls[offX + nx][offY + ny])
                {
                    available.push_back({ nx, ny });
                }
            }

            if (!available.empty())
            {
               
                std::uniform_int_distribution<int> pick(0, (int)available.size() - 1);
                auto [nx, ny] = available[pick(rng)];

         
                int wallX = (cx + nx) / 2;
                int wallY = (cy + ny) / 2;

                walls[offX + wallX][offY + wallY] = false;
                walls[offX + nx][offY + ny] = false;

                stack.push({ nx, ny });
            }
            else
            {
               
                stack.pop();
            }
        }
    }

    void RoguelikeMaze::CarveConnections(std::mt19937& rng)
    {
        std::uniform_int_distribution<int> yDist(0, 1);
        std::uniform_int_distribution<int> xDist(0, 1);

        int y1 = (yDist(rng) == 0) ? 5 : 7;
        int x1 = (xDist(rng) == 0) ? 9 : 11;
        int x2 = (xDist(rng) == 0) ? 9 : 11;
        int y2 = (yDist(rng) == 0) ? 5 : 7;

        walls[20][12 + y1] = false;
        walls[x1][12] = false;
        walls[20 + x2][12] = false;
        walls[20][y2] = false;
    }

    bool RoguelikeMaze::IsWall(int gx, int gy) const
    {
        if (gx < 0 || gx >= GLOBAL_CELLS_X || gy < 0 || gy >= GLOBAL_CELLS_Y) return true;
        return walls[gx][gy];
    }

    bool RoguelikeMaze::IsWallWorld(float x, float y) const
    {
        int gx = (int)(x / CELL_W);
        int gy = (int)(y / CELL_H);
        return IsWall(gx, gy);
    }

    bool RoguelikeMaze::IsWalkableWorld(float x, float y, float r) const
    {
        const float eps = 0.5f;
        if (IsWallWorld(x - r + eps, y - r + eps)) return false;
        if (IsWallWorld(x + r - eps, y - r + eps)) return false;
        if (IsWallWorld(x - r + eps, y + r - eps)) return false;
        if (IsWallWorld(x + r - eps, y + r - eps)) return false;
        return true;
    }

    int RoguelikeMaze::BlockAt(float x, float y) const
    {
        int bc = (x >= BLOCK_W) ? 1 : 0;
        int br = (y >= BLOCK_H) ? 1 : 0;
        return br * 2 + bc;
    }

    Engine::Vector2Df RoguelikeMaze::BlockOrigin(int block) const
    {
        int bc = block % 2, br = block / 2;
        return { bc * BLOCK_W, br * BLOCK_H };
    }

    Engine::Vector2Df RoguelikeMaze::BlockCenterWorld(int block) const
    {
        Engine::Vector2Df o = BlockOrigin(block);
        return { o.x + BLOCK_W * 0.5f, o.y + BLOCK_H * 0.5f };
    }

    Engine::Vector2Df RoguelikeMaze::GlobalCellCenter(int gx, int gy) const
    {
        return { (gx + 0.5f) * CELL_W, (gy + 0.5f) * CELL_H };
    }

    bool RoguelikeMaze::WorldToGlobal(float x, float y, int& gx, int& gy) const
    {
        gx = (int)(x / CELL_W);
        gy = (int)(y / CELL_H);
        return gx >= 0 && gx < GLOBAL_CELLS_X && gy >= 0 && gy < GLOBAL_CELLS_Y;
    }

    Engine::Vector2Df RoguelikeMaze::LocalRoomCenter(int block, int localX, int localY) const
    {
        int bc = block % 2, br = block / 2;
        int gx = bc * CELLS_PER_BLOCK_X + localX;
        int gy = br * CELLS_PER_BLOCK_Y + localY;
        return GlobalCellCenter(gx, gy);
    }

    Engine::Vector2Df RoguelikeMaze::RandomRoomCenter(int blockFilter) const
    {
        static std::mt19937 rng(seed ^ 0xDEADBEEF);
        std::uniform_int_distribution<int> bDist(0, 3);
        std::uniform_int_distribution<int> xDist(0, 9);
        std::uniform_int_distribution<int> yDist(0, 5);

        for (int attempt = 0; attempt < 500; ++attempt)
        {
            int b = (blockFilter >= 0) ? blockFilter : bDist(rng);
            int lx = 1 + 2 * xDist(rng);
            int ly = 1 + 2 * yDist(rng);
            if (lx >= CELLS_PER_BLOCK_X || ly >= CELLS_PER_BLOCK_Y) continue;

            int bc = b % 2, br = b / 2;
            int gx = bc * CELLS_PER_BLOCK_X + lx;
            int gy = br * CELLS_PER_BLOCK_Y + ly;
            if (!walls[gx][gy])
                return GlobalCellCenter(gx, gy);
        }
        return LocalRoomCenter(0, 1, 1);
    }

    std::vector<Engine::Vector2Df> RoguelikeMaze::FindPath(Engine::Vector2Df fromWorld, Engine::Vector2Df toWorld) const
    {
        int fx, fy, tx, ty;
        if (!WorldToGlobal(fromWorld.x, fromWorld.y, fx, fy)) return {};
        if (!WorldToGlobal(toWorld.x, toWorld.y, tx, ty)) return {};

        if (walls[fx][fy] || walls[tx][ty]) return {};
        if (fx == tx && fy == ty) return { GlobalCellCenter(fx, fy) };

        auto h = [tx, ty](int x, int y) { return (float)(std::abs(x - tx) + std::abs(y - ty)); };

        struct Node { float f; int x, y; };
        struct Cmp { bool operator()(const Node& a, const Node& b) const { return a.f > b.f; } };
        std::priority_queue<Node, std::vector<Node>, Cmp> open;

        std::vector<std::vector<float>> g(GLOBAL_CELLS_X, std::vector<float>(GLOBAL_CELLS_Y, 1e9f));
        std::vector<std::vector<std::pair<int, int>>> parent(GLOBAL_CELLS_X,
            std::vector<std::pair<int, int>>(GLOBAL_CELLS_Y, { -1, -1 }));
        std::vector<std::vector<bool>> closed(GLOBAL_CELLS_X,
            std::vector<bool>(GLOBAL_CELLS_Y, false));

        g[fx][fy] = 0.f;
        open.push({ h(fx, fy), fx, fy });

        const int dirs[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
        bool reached = false;

        while (!open.empty())
        {
            auto [f, x, y] = open.top(); open.pop();
            if (closed[x][y]) continue;
            closed[x][y] = true;
            if (x == tx && y == ty) { reached = true; break; }

            for (auto& d : dirs)
            {
                int nx = x + d[0], ny = y + d[1];
                if (nx < 0 || nx >= GLOBAL_CELLS_X || ny < 0 || ny >= GLOBAL_CELLS_Y) continue;
                if (walls[nx][ny] || closed[nx][ny]) continue;
                float ng = g[x][y] + 1.f;
                if (ng < g[nx][ny])
                {
                    g[nx][ny] = ng;
                    parent[nx][ny] = { x, y };
                    open.push({ ng + h(nx, ny), nx, ny });
                }
            }
        }

        if (!reached) return {};

        std::vector<Engine::Vector2Df> path;
        int cx = tx, cy = ty;
        while (cx != -1 && cy != -1)
        {
            path.push_back(GlobalCellCenter(cx, cy));
            if (cx == fx && cy == fy) break;
            auto p = parent[cx][cy];
            cx = p.first; cy = p.second;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
}