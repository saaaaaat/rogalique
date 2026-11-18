#include "pch.h"
#include "ExitComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "Logger.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    ExitComponent::ExitComponent(GameObject* obj) : Component(obj) {}

    void ExitComponent::Update(float dt)
    {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver() || orch.isBossRoom) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        auto* player = orch.GetPlayer();
        if (!t || !player) return;
        auto* pt = player->GetComponent<TransformComponent>();
        if (!pt) return;

        float d = (t->GetWorldPosition() - pt->GetWorldPosition()).GetLength();
        if (d < TRIGGER_RADIUS)
        {
            if (orch.hasHalfKey1 && orch.hasHalfKey2)
            {
                LOG_INFO("[Exit] Door opened. win");
                orch.SetGameOver(true);
            }
            else
            {
                static float warnCooldown = 0.f;
                warnCooldown -= dt;
                if (warnCooldown <= 0.f) {
                    LOG_WARN("[Exit] Locked. Need both halves of the key");
                    warnCooldown = 2.f;
                }
            }
        }
    }
}