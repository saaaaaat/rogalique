#include "pch.h"
#include "ProjectileComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "GameWorld.h"
#include "Logger.h"
#include <cassert>

using namespace Engine;

namespace RogaliqueGame {
    ProjectileComponent::ProjectileComponent(GameObject* obj, Engine::Vector2Df v, float life)
        : Component(obj), velocity(v), lifetime(life)
    {
        assert(obj != nullptr && "ProjectileComponent requires valid GameObject");
        assert(life > 0.f && "ProjectileComponent: lifetime must be > 0");
    }

    void ProjectileComponent::Update(float dt) {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver()) return;

        lifetime -= dt;
        if (lifetime <= 0.f) {
            GameWorld::Instance()->DestroyGameObject(GetGameObject());
            return;
        }

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;
        t->MoveBy(velocity.x * dt, velocity.y * dt);


        if (orch.bossDefeated) return;

        auto* p = orch.GetPlayer();
        if (!p) return;
        auto* pt = p->GetComponent<TransformComponent>();
        auto* ph = p->GetComponent<HealthComponent>();
        if (!pt || !ph || ph->IsDead()) return;

        float d = (t->GetWorldPosition() - pt->GetWorldPosition()).GetLength();
        if (d < 24.f) {
            ph->TakeDamage(15);
            LOG_WARN("Player hit by bullet HP: " + std::to_string(ph->GetHp()));
            GameWorld::Instance()->DestroyGameObject(GetGameObject());
        }
    }
}