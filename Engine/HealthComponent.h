#pragma once
#include "Component.h"
#include <string>
#include <cassert>

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

    
        int TakeDamage(int damage);
        void Heal(int amount);
        bool IsDead() const { return hp <= 0; }

        int GetHp()      const { return hp; }
        int GetMaxHp()   const { return maxHp; }
        int GetArmor()   const { return armor; }
        int GetMaxArmor()const { return maxArmor; }

    private:
        int hp;
        int maxHp;
        int armor;
        int maxArmor;
    };
}