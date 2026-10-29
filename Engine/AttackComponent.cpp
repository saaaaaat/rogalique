#include "pch.h"
#include "AttackComponent.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "Logger.h"
#include <cmath>
#include <iostream>
#include <cassert>

namespace Engine
{
    void AttackComponent::Update(float deltaTime)
    {
        if (GetGameObject()->GetName() != "Player") return;

        cooldown -= deltaTime;
        if (cooldown > 0.0f) return;

        auto gameObjects = GameWorld::Instance()->GetGameObjects();
        GameObject* target = nullptr;

        for (auto obj : gameObjects) {
            if (obj->GetName() == "Enemy") { target = obj; break; }
        }

        // Ассерт: если мы игрок, враг должен быть в мире
        assert(target != nullptr && "ERROR: No Enemy found for Player to attack!");

        if (!target || target == GetGameObject()) return;

        auto myTransform = GetGameObject()->GetComponent<TransformComponent>();
        auto targetTransform = target->GetComponent<TransformComponent>();
        auto myHealth = GetGameObject()->GetComponent<HealthComponent>();
        auto targetHealth = target->GetComponent<HealthComponent>();

        // Ассерт на здоровье
        assert(myHealth != nullptr && "ERROR: Player has no HealthComponent!");
        assert(targetHealth != nullptr && "ERROR: Enemy has no HealthComponent!");

        if (myHealth->IsDead() || targetHealth->IsDead()) return;

        if (myTransform && targetTransform && myHealth && targetHealth)
        {
            Vector2Df pPos = myTransform->GetWorldPosition();
            Vector2Df ePos = targetTransform->GetWorldPosition();
            float dist = std::sqrt(std::pow(pPos.x - ePos.x, 2) + std::pow(pPos.y - ePos.y, 2));

            if (dist < 45.f)
            {
                std::cout << "--- !!! PLAYER COMBO !!! ---" << std::endl;
                LOG_INFO("--- !!! PLAYER COMBO !!! ---");

                targetHealth->TakeDamage(10);
                std::cout << "Player MAIN hit (10 dmg)!" << std::endl;
                LOG_INFO("Player MAIN hit (10 dmg)!");

                targetHealth->TakeDamage(5);
                std::cout << "Player BONUS hit (5 dmg)!" << std::endl;
                LOG_INFO("Player BONUS hit (5 dmg)!");

                myHealth->TakeDamage(20);
                std::cout << "Enemy hits back (20 dmg)!" << std::endl;
                LOG_WARN("Enemy hits back (20 dmg)!");

                cooldown = 5.0f;
            }
        }
    }
}