#include "pch.h"
#include "ChaseComponent.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "Logger.h"
#include <cmath>
#include <cassert>

namespace Engine
{
    void ChaseComponent::Update(float deltaTime)
    {
        GameObject* target = nullptr;
        for (auto* obj : GameWorld::Instance()->GetGameObjects())
        {
            if (obj == GetGameObject()) continue;
            if (obj->GetName() == "Player") { target = obj; break; }
        }
        if (!target) return;

        auto* targetT = target->GetComponent<TransformComponent>();
        auto* myT = GetGameObject()->GetComponent<TransformComponent>();
        auto* myRb = GetGameObject()->GetComponent<RigidbodyComponent>();

        assert(myT != nullptr && "ChaseComponent: owner missing TransformComponent");
        assert(myRb != nullptr && "ChaseComponent: owner missing RigidbodyComponent");
        if (!targetT || !myT || !myRb) return;

        Vector2Df direction = targetT->GetWorldPosition() - myT->GetWorldPosition();
        float length = direction.GetLength();

        if (length <= stopDistance) {
            if (myRb->GetLinearVelocity().GetLength() > 0.1f) {
                LOG_INFO(GetGameObject()->GetName() + " reached stop distance ("
                    + std::to_string(length) + " <= " + std::to_string(stopDistance)
                    + "), halting");
                myRb->SetLinearVelocity({ 0.f, 0.f });
            }
            return;
        }

        if (length > 0.f) {
            direction.x /= length;
            direction.y /= length;
            myRb->SetLinearVelocity(direction * speed);
        }
    }
}