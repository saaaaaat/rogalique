#include "Player.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include <ResourceSystem.h>
#include <cassert>

namespace RogaliqueGame
{
    Player::Player()
    {
        LOG_INFO("Creating Player...");
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");
        assert(gameObject != nullptr && "Failed to create Player GameObject");
        if (!gameObject) { LOG_ERROR("Player: GameObject creation failed"); return; }

        startPosition = sf::Vector2f(640.f, 360.f);

        auto* renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        assert(renderer && "Player: SpriteRendererComponent add failed");
        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("ball"));
        renderer->SetPixelSize(32, 32);

        auto* cam = gameObject->AddComponent<Engine::CameraComponent>();
        cam->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
        cam->SetBaseResolution(1280, 720);

        auto* transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(startPosition.x, startPosition.y);

        auto* body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetLinearDamping(20.f);

        gameObject->AddComponent<Engine::SpriteColliderComponent>();
        gameObject->AddComponent<Engine::InputComponent>();
        gameObject->AddComponent<Engine::HealthComponent>(100, 30);

      

        LOG_INFO("Player created: HP=100 ARMOR=30 (no AttackComponent by design)");
    }

    Engine::GameObject* Player::GetGameObject() { return gameObject; }
}