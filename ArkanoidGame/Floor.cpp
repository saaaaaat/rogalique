#include "Floor.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "Logger.h"
#include <memory>

namespace RogaliqueGame
{
    Floor::Floor(sf::Vector2f position, sf::Vector2f size)
    {
        static std::shared_ptr<sf::Texture> floorTexture = []() {
            auto* tex = new sf::Texture();
            sf::Image img;
            img.create(1, 1, sf::Color(105, 105, 105));
            tex->loadFromImage(img);
            return std::shared_ptr<sf::Texture>(tex);
            }();

        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Floor");

        auto* renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        renderer->SetTexture(*floorTexture);
        renderer->SetPixelSize((int)size.x, (int)size.y);

        auto* transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(position.x, position.y);

        LOG_INFO("Floor created at (" + std::to_string(position.x) + ","
            + std::to_string(position.y) + ") size "
            + std::to_string(size.x) + "x" + std::to_string(size.y));
    }
}