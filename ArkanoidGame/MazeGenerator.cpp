#include "pch.h"
#include "MazeGenerator.h"
#include "DeveloperLevel.h"
#include "GameWorld.h"
#include <cstdlib>
#include <ctime>

namespace RogaliqueGame
{
    MazeGenerator::MazeGenerator(int width, int height, DeveloperLevel* level)
        : width(width), height(height), level(level)
    {
        grid.resize(height, std::vector<bool>(width, false));
    }

    void MazeGenerator::Generate()
    {
        std::srand(std::time(nullptr));

        int startX = std::rand() % width;
        int startY = std::rand() % height;

        std::stack<std::pair<int, int>> stack;
        stack.push({ startX, startY });
        grid[startY][startX] = true;

        while (!stack.empty())
        {
            auto [x, y] = stack.top();
            stack.pop();

            std::vector<std::pair<int, int>> directions = GetAvailableDirections(x, y);

            if (!directions.empty())
            {
                stack.push({ x, y });
                std::pair<int, int> dir = directions[std::rand() % directions.size()];
                int nx = x + dir.first;
                int ny = y + dir.second;

                RemoveWall(x, y, nx, ny);

                grid[ny][nx] = true;
                stack.push({ nx, ny });
            }
        }
    }

    std::vector<std::pair<int, int>> MazeGenerator::GetAvailableDirections(int x, int y)
    {
        std::vector<std::pair<int, int>> directions = {
            {0, -2}, {0, 2}, {-2, 0}, {2, 0}
        };

        std::vector<std::pair<int, int>> available;

        for (const auto& dir : directions)
        {
            int nx = x + dir.first;
            int ny = y + dir.second;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height && !grid[ny][nx])
            {
                available.push_back(dir);
            }
        }

        return available;
    }

    void MazeGenerator::RemoveWall(int x1, int y1, int x2, int y2)
    {
        int wallX = (x1 + x2) / 2;
        int wallY = (y1 + y2) / 2;

      
        float cellSize = 128.f;

       
        float floorSize = 126.f;


        float wallSize = 120.f;

        level->floors.push_back(std::make_unique<Floor>(
            sf::Vector2f(x1 * cellSize + cellSize / 2, y1 * cellSize + cellSize / 2),
            sf::Vector2f(floorSize, floorSize)
        ));

        level->floors.push_back(std::make_unique<Floor>(
            sf::Vector2f(x2 * cellSize + cellSize / 2, y2 * cellSize + cellSize / 2),
            sf::Vector2f(floorSize, floorSize)
        ));

      
        if (wallX != x1 || wallY != y1)
        {
            level->walls.push_back(std::make_unique<Wall>(
                sf::Vector2f(wallX * cellSize + cellSize / 2, wallY * cellSize + cellSize / 2),
                sf::Vector2f(wallSize, wallSize)
            ));
        }
    }
}