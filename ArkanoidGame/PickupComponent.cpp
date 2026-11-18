#include "pch.h"
#include "PickupComponent.h"
#include "GameOrchestrator.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "Logger.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    const char* ItemTypeToString(ItemType t)
    {
        switch (t)
        {
        case ItemType::Clone: return "Clone";
        case ItemType::Sword: return "Sword";
        case ItemType::Speed: return "Speed";
        case ItemType::Key1:  return "Half Key 1";
        case ItemType::Key2:  return "Half Key 2";
        case ItemType::PurpleKey: return "Purple Key";
        case ItemType::HealthPotion: return "Health Potion";
        case ItemType::InvisibilityPotion: return "Invisibility Potion";
        }
        return "?";
    }

    PickupComponent::PickupComponent(GameObject* obj, ItemType t)
        : Component(obj), type(t)
    {
        LOG_INFO(std::string("[Item] Spawned: ") + ItemTypeToString(type));
    }

    void PickupComponent::Update(float dt)
    {
        auto& orch = GameOrchestrator::Instance();
        if (orch.IsGameOver()) return;

        auto* t = GetGameObject()->GetComponent<TransformComponent>();

        auto* player = orch.GetPlayer();
        if (!t || !player) return;
        auto* pt = player->GetComponent<TransformComponent>();
        if (!pt) return;

        float dist = (t->GetWorldPosition() - pt->GetWorldPosition()).GetLength();
        if (dist < PICKUP_RADIUS)
        {
            LOG_INFO(std::string("[Player] Picked up: ") + ItemTypeToString(type));
            orch.OnPickup(type);
            GameWorld::Instance()->DestroyGameObject(GetGameObject());
        }
    }
}