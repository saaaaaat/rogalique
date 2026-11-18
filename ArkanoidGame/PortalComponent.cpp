#include "pch.h"
#include "PortalComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "Logger.h"
using namespace Engine;
namespace RogaliqueGame {
    PortalComponent::PortalComponent(GameObject* obj) : Component(obj) {}

    void PortalComponent::Update(float dt) {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver() || orch.isBossRoom) return;

        if (orch.hasHalfKey2) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();
        auto* p = orch.GetPlayer();
        if (!t || !p) return;
        auto* pt = p->GetComponent<TransformComponent>();
        if (!pt) return;

        float d = (t->GetWorldPosition() - pt->GetWorldPosition()).GetLength();
        if (d < 36.f) {
            if (orch.hasPurpleKey) {
                LOG_INFO("Entering boss room");
                orch.EnterBossRoom();
            }
            else {
                static float warn = 0.f; warn -= dt;
                if (warn <= 0.f) {
                    LOG_WARN("Portal Locked. Need Purple Key ");
                    warn = 2.f;
                }
            }
        }
    }
}