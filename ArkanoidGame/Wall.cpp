#include "Wall.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "ResourceSystem.h"
#include <memory>

namespace RogaliqueGame {
    Wall::Wall(sf::Vector2f position, sf::Vector2f size) {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Wall");

        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

        static std::shared_ptr<sf::Texture> wallTexture = []() {
            auto tex = std::make_shared<sf::Texture>();
            sf::Image img;
            img.create(1, 1, sf::Color(50, 50, 150));
            tex->loadFromImage(img);
            return tex;
            }();

       

        renderer->SetTexture(*wallTexture);
        renderer->SetPixelSize((int)size.x, (int)size.y);

        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(position.x, position.y);

        auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetKinematic(true);

        gameObject->AddComponent<Engine::SpriteColliderComponent>();
    }
}