#include "Enemy.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "ChaseComponent.h"
#include "AttackComponent.h"
#include "Logger.h"
#include "HealthComponent.h"

namespace RogaliqueGame {
    Enemy::Enemy() {
        LOG_INFO("Creating enemy...");
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Enemy");

        startPosition = sf::Vector2f(1100.f, 100.f);

        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
        renderer->SetPixelSize(32, 32);
        renderer->SetColor(sf::Color::Red);

        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(startPosition.x, startPosition.y);

        auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetLinearDamping(10.f);

        gameObject->AddComponent<Engine::SpriteColliderComponent>();
        gameObject->AddComponent<Engine::ChaseComponent>();

        gameObject->AddComponent<Engine::HealthComponent>(150, 0);
        //gameObject->AddComponent<Engine::AttackComponent>();

        LOG_INFO("Enemy created successfully! Stats: HP = 150, Armor = 0, Damage = 20");
    }
}