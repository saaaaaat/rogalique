#include "pch.h"
#include "ExploderComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "CircleRendererComponent.h"
#include "GameWorld.h"
#include "Logger.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    ExploderComponent::ExploderComponent(GameObject* obj) : Component(obj)
    {
        LOG_INFO("[Exploder] Spawned");
    }

    void ExploderComponent::Update(float dt)
    {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver()) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;
        auto* player = orch.GetPlayer();
        if (!player) return;
        auto* pt = player->GetComponent<TransformComponent>();
        if (!pt) return;

        auto dist = (t->GetWorldPosition() - pt->GetWorldPosition()).GetLength();

        if (!armed && dist < TRIGGER_RADIUS)
        {
            armed = true;
            countdown = COUNTDOWN;
            LOG_WARN("[Exploder] Armed! Countdown 2.0s");
            if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
                r->SetColor(sf::Color(255, 100, 0));
        }

        if (armed)
        {
            countdown -= dt;
     
            if (auto* r = GetGameObject()->GetComponent<CircleRendererComponent>())
            {
                int phase = (int)((COUNTDOWN - countdown) * 4.f) % 2;
                r->SetColor(phase == 0 ? sf::Color(255, 0, 0) : sf::Color(255, 255, 0));
            }

            if (countdown <= 0.f) Explode();
        }
    }

    void ExploderComponent::Explode()
    {
        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;
        Engine::Vector2Df pos = t->GetWorldPosition();
        auto& orch = GameOrchestrator::Instance();

        auto* player = orch.GetPlayer();
        if (player)
        {
            auto* pt = player->GetComponent<TransformComponent>();
            auto* ph = player->GetComponent<HealthComponent>();
            if (pt && ph)
            {
                float d = (pos - pt->GetWorldPosition()).GetLength();
                if (d < EXPLODE_RADIUS)
                {
                    ph->TakeDamage(EXPLODE_DAMAGE);
                    LOG_ERROR("[Exploder] BLAST hit player for " + std::to_string(EXPLODE_DAMAGE));
                }
                else
                {
                    LOG_INFO("[Exploder] Player escaped the blast (dist=" + std::to_string((int)d) + ")");
                }
            }
        }
        LOG_ERROR("[Exploder] Detonated at (" + std::to_string((int)pos.x) + "," + std::to_string((int)pos.y) + ")");
        GameWorld::Instance()->DestroyGameObject(GetGameObject());
    }
}