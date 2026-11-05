#include "pch.h"
#include "AttackComponent.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "DamageIndicatorComponent.h"
#include "Logger.h"
#include <cmath>
#include <cassert>
#include <stdexcept>

namespace Engine
{
    AttackComponent::AttackComponent(GameObject* gameObject, int damage, float attackRange, float cooldownTime)
        : Component(gameObject), damage(damage), attackRange(attackRange), cooldownTime(cooldownTime)
    {
        if (damage < 0)         throw std::invalid_argument("AttackComponent: damage < 0");
        if (attackRange <= 0.f) throw std::invalid_argument("AttackComponent: attackRange <= 0");
        if (cooldownTime <= 0.f)throw std::invalid_argument("AttackComponent: cooldownTime <= 0");

        LOG_INFO("AttackComponent on '" + gameObject->GetName()
            + "' dmg=" + std::to_string(damage)
            + " range=" + std::to_string(attackRange)
            + " cd=" + std::to_string(cooldownTime));
    }

    void AttackComponent::Update(float deltaTime)
    {
        cooldown -= deltaTime;
        if (cooldown > 0.f) return;

        GameObject* self = GetGameObject();
        GameObject* target = nullptr;
        for (auto* obj : GameWorld::Instance()->GetGameObjects())
        {
            if (obj == self) continue;
            if (obj->GetName() == "Player") { target = obj; break; }
        }
        if (!target) return;

        auto* myT = self->GetComponent<TransformComponent>();
        auto* tarT = target->GetComponent<TransformComponent>();
        auto* myH = self->GetComponent<HealthComponent>();
        auto* tarH = target->GetComponent<HealthComponent>();

        assert(myT != nullptr && "AttackComponent: owner missing TransformComponent");
        assert(myH != nullptr && "AttackComponent: owner missing HealthComponent");
        if (!tarT || !tarH) return;
        if (myH->IsDead() || tarH->IsDead()) return;

        Vector2Df a = myT->GetWorldPosition();
        Vector2Df b = tarT->GetWorldPosition();
        float dx = a.x - b.x, dy = a.y - b.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        LOG_INFO(self->GetName() + " attacks " + target->GetName()
            + " | dist=" + std::to_string(dist) + " range=" + std::to_string(attackRange));

        if (dist > attackRange)
        {
            LOG_INFO("  ...too far, no hit");
            return;
        }

        int hpDamage = tarH->TakeDamage(damage);
        LOG_INFO("  HIT! " + target->GetName() + " hp dmg: " + std::to_string(hpDamage));
        SpawnDamageIndicator(b, hpDamage > 0 ? hpDamage : damage);
        cooldown = cooldownTime;
        LOG_INFO("  cooldown reset to " + std::to_string(cooldownTime) + "s");
    }

    void AttackComponent::SpawnDamageIndicator(const Vector2Df& worldPos, int amount) const
    {
        auto* world = GameWorld::Instance();
        auto* obj = world->CreateGameObject("DamageIndicator");
        if (!obj) { LOG_ERROR("Failed to spawn DamageIndicator GameObject"); return; }

        auto* t = obj->GetComponent<TransformComponent>();
        if (!t) { LOG_ERROR("DamageIndicator missing TransformComponent"); return; }
        t->SetWorldPosition(worldPos);

        obj->AddComponent<DamageIndicatorComponent>(std::to_string(amount), sf::Color::Red);
        LOG_INFO("Damage indicator '" + std::to_string(amount) + "' spawned at ("
            + std::to_string(worldPos.x) + "," + std::to_string(worldPos.y) + ")");
    }
}