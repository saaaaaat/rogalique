#include "pch.h"
#include "MazeRendererComponent.h"
#include "Logger.h"
#include <assert.h>

using namespace Engine;

namespace RogaliqueGame
{
    MazeRendererComponent::MazeRendererComponent(Engine::GameObject* obj, const RoguelikeMaze* m)
        : Component(obj), maze(m)
    {
        assert(maze != nullptr && "MazeRendererComponent: null maze");

        sf::Image img;
        img.create(RoguelikeMaze::GLOBAL_CELLS_X, RoguelikeMaze::GLOBAL_CELLS_Y, sf::Color(30, 30, 60));
        for (int x = 0; x < RoguelikeMaze::GLOBAL_CELLS_X; x++)
            for (int y = 0; y < RoguelikeMaze::GLOBAL_CELLS_Y; y++)
                if (!maze->IsWall(x, y))
                    img.setPixel(x, y, sf::Color(90, 90, 95));

        texture = new sf::Texture();
        texture->loadFromImage(img);
        texture->setSmooth(false);
        sprite.setTexture(*texture);
        sprite.setScale(RoguelikeMaze::CELL_W, RoguelikeMaze::CELL_H);
        sprite.setPosition(0.f, 0.f);
    }

    MazeRendererComponent::~MazeRendererComponent()
    {
        delete texture;
        texture = nullptr;
    }

    void MazeRendererComponent::Render()
    {
        if (texture) Engine::RenderSystem::Instance()->Render(sprite);
    }
}