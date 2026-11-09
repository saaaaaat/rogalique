#include "pch.h"
#include "ChaseAIComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CircleRendererComponent.h"
#include "Logger.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    ChaseAIComponent::ChaseAIComponent(GameObject* obj) : Component(obj)
    {
        LOG_INFO("[Chaser] AI attached (speed=" + std::to_string((int)SPEED) + ")");
    }

    void ChaseAIComponent::Update(float dt)
    {
        auto& orch = GameOrchestrator::Instance();
        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;

        if (orch.IsGameOver()) return;

       
        if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
            r->SetColor(stunTimer > 0.f ? sf::Color(150, 150, 255) : sf::Color(220, 60, 60));

        if (stunTimer > 0.f)
        {
            stunTimer -= dt;
            return;
        }

        auto* target = orch.GetCloneOrPlayer();
        if (!target) return;
        auto* tt = target->GetComponent<TransformComponent>();
        if (!tt) return;

        Engine::Vector2Df targetPos = tt->GetWorldPosition();
        Engine::Vector2Df myPos = t->GetWorldPosition();

        auto path = orch.GetMaze().FindPath(myPos, targetPos);
        Engine::Vector2Df desired;
        if (path.size() <= 1)
            desired = targetPos;
        else
            desired = path[1];

        Engine::Vector2Df delta = desired - myPos;
        float len = delta.GetLength();

        if (len > 0.5f)
        {
            delta.x /= len; delta.y /= len;
            Engine::Vector2Df newPos = myPos + delta * (SPEED * dt);
            auto& maze = orch.GetMaze();
            if (maze.IsWalkableWorld(newPos.x, newPos.y, RADIUS))
                t->SetWorldPosition(newPos.x, newPos.y);
            else if (maze.IsWalkableWorld(newPos.x, myPos.y, RADIUS))
                t->SetWorldPosition(newPos.x, myPos.y);
            else if (maze.IsWalkableWorld(myPos.x, newPos.y, RADIUS))
                t->SetWorldPosition(myPos.x, newPos.y);
        }

     
        if (target == orch.GetPlayer())
        {
            contactTimer -= dt;
            if (contactTimer <= 0.f)
            {
                float dist = (t->GetWorldPosition() - targetPos).GetLength();
                if (dist < CONTACT_RADIUS)
                {
                    if (auto* h = target->GetComponent<HealthComponent>())
                    {
                        h->TakeDamage(CONTACT_DAMAGE);
                        LOG_WARN("[Chaser] Hit player for " + std::to_string(CONTACT_DAMAGE) + " HP");
                    }
                    contactTimer = CONTACT_INTERVAL;
                }
            }
        }
    }
}