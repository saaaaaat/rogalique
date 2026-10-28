#include "pch.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include <algorithm>
#include <iostream>

namespace Engine
{
    HealthComponent::HealthComponent(GameObject* gameObject, int maxHp, int armorBonus)
        : Component(gameObject), hp(maxHp), maxHp(maxHp), armor(armorBonus), maxArmor(armorBonus)
    {
    }

    void HealthComponent::TakeDamage(int damage)
    {
        if (damage <= 0) return;

        if (armor > 0) {
            int armorDamage = std::min(damage, armor);
            armor -= armorDamage;
            damage -= armorDamage;

            if (armor == 0) {
                std::cout << GetGameObject()->GetName() << " armor BROKEN!" << std::endl;
            }
        }

        if (damage > 0) {
            hp -= damage;
            if (hp < 0) hp = 0;
        }

        std::cout << GetGameObject()->GetName() << " -> HP: " << hp << ", Armor: " << armor << std::endl;

        if (hp <= 0) {
            std::cout << GetGameObject()->GetName() << " DESTROYED!" << std::endl;
        }
    }
}