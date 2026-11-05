#pragma once
#include "Component.h"

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    class ENGINE_API ChaseComponent : public Component
    {
    public:
        ChaseComponent(GameObject* gameObject, float stopDistance = 55.f, float speed = 120.f)
            : Component(gameObject), stopDistance(stopDistance), speed(speed) {
        }

        void Update(float deltaTime) override;
        void Render() override {}

    private:
        float stopDistance;
        float speed;
    };
}