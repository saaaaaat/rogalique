#include "pch.h"
#include "AttackComponent.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include <cmath>
#include <iostream>

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

        if (!target || target == GetGameObject()) return;

        auto myTransform = GetGameObject()->GetComponent<TransformComponent>();
        auto targetTransform = target->GetComponent<TransformComponent>();
        auto myHealth = GetGameObject()->GetComponent<HealthComponent>();
        auto targetHealth = target->GetComponent<HealthComponent>();

        // НОВАЯ ПРОВЕРКА: Если кто-то из них мертв — не бьем!
        if (myHealth->IsDead() || targetHealth->IsDead()) return;

        if (myTransform && targetTransform && myHealth && targetHealth)
        {
            Vector2Df pPos = myTransform->GetWorldPosition();
            Vector2Df ePos = targetTransform->GetWorldPosition();
            float dist = std::sqrt(std::pow(pPos.x - ePos.x, 2) + std::pow(pPos.y - ePos.y, 2));

            if (dist < 45.f)
            {
                std::cout << "--- !!! PLAYER COMBO !!! ---" << std::endl;

                targetHealth->TakeDamage(10);
                std::cout << "Player MAIN hit (10 dmg)!" << std::endl;

                targetHealth->TakeDamage(5);
                std::cout << "Player BONUS hit (5 dmg)!" << std::endl;

                myHealth->TakeDamage(20);
                std::cout << "Enemy hits back (20 dmg)!" << std::endl;

                cooldown = 5.0f;
            }
        }
    }
}