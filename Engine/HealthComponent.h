#pragma once
#include "Component.h"
#include <string>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    class ENGINE_API HealthComponent : public Component
    {
    public:
        HealthComponent(GameObject* gameObject, int maxHp, int armorBonus);

        void Update(float deltaTime) override {}
        void Render() override {}

        void TakeDamage(int damage);
        bool IsDead() const { return hp <= 0; }

        int GetHp() const { return hp; }
        int GetArmor() const { return armor; }

    private:
        int hp;
        int maxHp;
        int armor;
        int maxArmor;
    };
}