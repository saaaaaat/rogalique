#include "pch.h"
#include "ExploderComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CircleRendererComponent.h"
#include "GameWorld.h"
#include "Logger.h"
using namespace Engine;
namespace RogaliqueGame {
    ExploderComponent::ExploderComponent(GameObject* obj) : Component(obj) {}
    void ExploderComponent::Update(float dt) {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver() || orch.invisibilityTimer > 0.f) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        auto* p = orch.GetPlayer();
        if (!t || !p) return;

        float dist = (t->GetWorldPosition() - p->GetComponent<TransformComponent>()->GetWorldPosition()).GetLength();
        if (!armed && dist < TRIGGER_RADIUS) { armed = true; countdown = COUNTDOWN; }
        if (armed) {
            countdown -= dt;
            if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
                r->SetColor(((int)((COUNTDOWN - countdown) * 4.f) % 2) == 0 ? sf::Color(255, 0, 0) : sf::Color(255, 255, 0));
            if (countdown <= 0.f) Explode();
        }
    }
    void ExploderComponent::Explode() {
        auto pos = GetGameObject()->GetComponent<TransformComponent>()->GetWorldPosition();
        auto* p = GameOrchestrator::Instance().GetPlayer();
        if (p && (pos - p->GetComponent<TransformComponent>()->GetWorldPosition()).GetLength() < EXPLODE_RADIUS)
            p->GetComponent<HealthComponent>()->TakeDamage(EXPLODE_DAMAGE);
        GameWorld::Instance()->DestroyGameObject(GetGameObject());
    }
}