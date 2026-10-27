#include "pch.h"
#include "AttackComponent.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "Logger.h"
#include <cmath>

namespace Engine
{
    void AttackComponent::Update(float deltaTime)
    {
        // Ждем 1 секунду между ударами, чтобы не убить мгновенно
        cooldown -= deltaTime;
        if (cooldown > 0.0f) return;

        auto gameObjects = GameWorld::Instance()->GetGameObjects();
        GameObject* enemy = nullptr;

        // Ищем врага по имени
        for (auto obj : gameObjects) {
            if (obj->GetName() == "Enemy") { enemy = obj; break; }
        }

        if (!enemy || enemy == GetGameObject()) return;

        auto myTransform = GetGameObject()->GetComponent<TransformComponent>();
        auto enemyTransform = enemy->GetComponent<TransformComponent>();
        auto myHealth = GetGameObject()->GetComponent<HealthComponent>();
        auto enemyHealth = enemy->GetComponent<HealthComponent>();

        if (myTransform && enemyTransform && myHealth && enemyHealth)
        {
            Vector2Df pPos = myTransform->GetWorldPosition();
            Vector2Df ePos = enemyTransform->GetWorldPosition();
            float dist = std::sqrt(std::pow(pPos.x - ePos.x, 2) + std::pow(pPos.y - ePos.y, 2));

            // Если расстояние меньше 45 пикселей (они столкнулись)
            if (dist < 45.f)
            {
                LOG_INFO("Столкновение! Игрок и Враг бьют друг друга.");

                // Наносим урон
                myHealth->TakeDamage(15);   // Враг бьет игрока
                enemyHealth->TakeDamage(5); // Игрок бьет врага

                cooldown = 1.0f; // Откат удара (1 секунда)
            }
        }
    }
}