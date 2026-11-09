#pragma once
#include "Component.h"
#include "RenderSystem.h"
#include "RoguelikeMaze.h"
#include <SFML/Graphics.hpp>

namespace RogaliqueGame
{
    class MazeRendererComponent : public Engine::Component
    {
    public:
        MazeRendererComponent(Engine::GameObject* obj, const RoguelikeMaze* maze);
        ~MazeRendererComponent() override;
        void Update(float) override {}
        void Render() override;
    private:
        const RoguelikeMaze* maze = nullptr;
        sf::Texture* texture = nullptr;
        sf::Sprite sprite;
    };
}