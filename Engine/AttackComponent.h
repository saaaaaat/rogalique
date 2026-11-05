#pragma once
#include "Component.h"
#include "Vector.h"

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    class ENGINE_API AttackComponent : public Component
    {
    public:
        AttackComponent(GameObject* gameObject,
            int damage = 10,
            float attackRange = 60.f,
            float cooldownTime = 1.5f);

        void Update(float deltaTime) override;
        void Render() override {}

    private:
        void SpawnDamageIndicator(const Vector2Df& worldPos, int amount) const;

        int   damage;
        float attackRange;
        float cooldownTime;
        float cooldown = 0.f;
    };
}