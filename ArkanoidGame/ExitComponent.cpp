#include "pch.h"
#include "ExitComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "Logger.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    ExitComponent::ExitComponent(GameObject* obj) : Component(obj)
    {
        LOG_INFO("[Exit] Spawned");
    }

    void ExitComponent::Update(float dt)
    {
        (void)dt;
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver()) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        if (!t) return;
        auto* player = orch.GetPlayer();
        if (!player) return;
        auto* pt = player->GetComponent<TransformComponent>();
        if (!pt) return;

        float d = (t->GetWorldPosition() - pt->GetWorldPosition()).GetLength();
        if (d < TRIGGER_RADIUS)
        {
            if (orch.hasKey)
            {
                LOG_INFO("[Exit] Door opened. YOU WIN!");
                orch.SetGameOver(true);
            }
            else
            {
                static float warnCooldown = 0.f;
                warnCooldown -= dt;
                if (warnCooldown <= 0.f)
                {
                    LOG_WARN("[Exit] Locked. Need key!");
                    warnCooldown = 2.f;
                }
            }
        }
    }
}