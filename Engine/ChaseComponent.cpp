#include "pch.h"
#include "ChaseComponent.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include <cmath>

namespace Engine {
    void ChaseComponent::Update(float deltaTime) {
        auto gameObjects = GameWorld::Instance()->GetGameObjects();
        GameObject* target = nullptr;
        for (auto obj : gameObjects) {
            if (obj->GetName() == "Player") { target = obj; break; }
        }
        if (!target) return;

        auto targetTransform = target->GetComponent<TransformComponent>();
        auto myTransform = GetGameObject()->GetComponent<TransformComponent>();
        auto myRigidbody = GetGameObject()->GetComponent<RigidbodyComponent>();

        if (targetTransform && myTransform && myRigidbody) {
            Vector2Df direction = targetTransform->GetWorldPosition() - myTransform->GetWorldPosition();
            float length = direction.GetLength();

            if (length > 0) {
                direction.x /= length;
                direction.y /= length;

                //скольжение по стене еси уперся
                auto currentVelocity = myRigidbody->GetLinearVelocity();
                if (currentVelocity.GetLength() < 10.0f && length > 50.0f) {
                    //смещение на 45
                    float tempX = direction.x;
                    direction.x = direction.x * 0.7f - direction.y * 0.7f;
                    direction.y = tempX * 0.7f + direction.y * 0.7f;
                }

                myRigidbody->SetLinearVelocity(direction * 120.0f);
            }
        }
    }
}