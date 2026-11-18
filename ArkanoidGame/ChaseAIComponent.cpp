#include "pch.h"
#include "ChaseAIComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CircleRendererComponent.h"
#include "Logger.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame {
    ChaseAIComponent::ChaseAIComponent(GameObject* obj) : Component(obj) {}
    void ChaseAIComponent::Update(float dt) {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver()) return;

        if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
            r->SetColor(stunTimer > 0.f ? sf::Color(150, 150, 255) : sf::Color(220, 60, 60));

        if (stunTimer > 0.f) { stunTimer -= dt; return; }
        if (orch.invisibilityTimer > 0.f) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        auto* target = orch.GetCloneOrPlayer();
        if (!t || !target) return;
        auto* tt = target->GetComponent<TransformComponent>();

        auto path = orch.GetMaze().FindPath(t->GetWorldPosition(), tt->GetWorldPosition());
        Engine::Vector2Df desired = (path.size() <= 1) ? tt->GetWorldPosition() : path[1];
        Engine::Vector2Df delta = desired - t->GetWorldPosition();
        float len = delta.GetLength();

        if (len > 0.5f) {
            delta.x /= len; delta.y /= len;
            Engine::Vector2Df newPos = t->GetWorldPosition() + delta * (SPEED * dt);
            auto& maze = orch.GetMaze();
            if (maze.IsWalkableWorld(newPos.x, newPos.y, RADIUS)) t->SetWorldPosition(newPos.x, newPos.y);
            else if (maze.IsWalkableWorld(newPos.x, t->GetWorldPosition().y, RADIUS)) t->SetWorldPosition(newPos.x, t->GetWorldPosition().y);
            else if (maze.IsWalkableWorld(t->GetWorldPosition().x, newPos.y, RADIUS)) t->SetWorldPosition(t->GetWorldPosition().x, newPos.y);
        }

        contactTimer -= dt;
        if (contactTimer <= 0.f && target == orch.GetPlayer()) {
            if ((t->GetWorldPosition() - tt->GetWorldPosition()).GetLength() < CONTACT_RADIUS) {
                tt->GetGameObject()->GetComponent<HealthComponent>()->TakeDamage(CONTACT_DAMAGE);
                contactTimer = CONTACT_INTERVAL;
            }
        }
    }
}