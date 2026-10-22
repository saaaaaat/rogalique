#include "Wall.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"

namespace RogaliqueGame {
    Wall::Wall(sf::Vector2f position, sf::Vector2f size) {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Wall");

        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

        sf::Texture* tex = new sf::Texture();
        sf::Image img;
        img.create(1, 1, sf::Color(50, 50, 150)); 
        tex->loadFromImage(img);

        renderer->SetTexture(*tex);
        renderer->SetPixelSize((int)size.x, (int)size.y);

        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(position.x, position.y);

        auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetKinematic(true);

        gameObject->AddComponent<Engine::SpriteColliderComponent>();
    }
}