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
#include <cassert>

namespace RogaliqueGame
{
    Enemy::Enemy()
    {
        LOG_INFO("Creating Enemy...");
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Enemy");
        assert(gameObject != nullptr && "Failed to create Enemy GameObject");
        if (!gameObject) { LOG_ERROR("Enemy: GameObject creation failed"); return; }

        startPosition = sf::Vector2f(1100.f, 100.f);

        auto* renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
        renderer->SetPixelSize(32, 32);
        renderer->SetColor(sf::Color::Red);

        auto* transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(startPosition.x, startPosition.y);

        auto* body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetLinearDamping(10.f);

        gameObject->AddComponent<Engine::SpriteColliderComponent>();
        gameObject->AddComponent<Engine::ChaseComponent>( 55.f,  120.f);
        gameObject->AddComponent<Engine::HealthComponent>(150, 0);
        gameObject->AddComponent<Engine::AttackComponent>(10, 60.f,  1.5f);

        LOG_INFO("Enemy created: HP=150 ARMOR=0 DMG=10 (attack enabled)");
    }
}