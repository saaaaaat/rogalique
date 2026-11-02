#include "pch.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "GameObject.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace Engine
{
    HealthComponent::HealthComponent(GameObject* gameObject, int maxHp, int armorBonus)
        : Component(gameObject), hp(maxHp), maxHp(maxHp), armor(armorBonus), maxArmor(armorBonus)
    {
        LOG_INFO("Created HealthComponent for " + gameObject->GetName() + ". HP: " + std::to_string(hp) + ", Armor: " + std::to_string(armor));
    }

    void HealthComponent::TakeDamage(int damage)
    {
        
        if (damage < 0) {
            throw std::invalid_argument("Damage cannot be negative!");
        }
        if (damage == 0) return;

        if (armor > 0) {
            int armorDamage = std::min(damage, armor);
            armor -= armorDamage;
            damage -= armorDamage;

            if (armor == 0) {
                LOG_WARN(GetGameObject()->GetName() + " armor BROKEN!");
                std::cout << GetGameObject()->GetName() << " armor BROKEN!" << std::endl;
            }
        }

        if (damage > 0) {
            hp -= damage;
            if (hp < 0) hp = 0;
        }

     
        std::cout << GetGameObject()->GetName() << " -> HP: " << hp << ", Armor: " << armor << std::endl;

        
        if (hp <= 0) {
            LOG_ERROR("Object " + GetGameObject()->GetName() + " DESTROYED!");
            std::cout << GetGameObject()->GetName() << " DESTROYED!" << std::endl;
        }
        else {
            LOG_INFO(GetGameObject()->GetName() + " took damage. HP left: " + std::to_string(hp));
        }
    }
}