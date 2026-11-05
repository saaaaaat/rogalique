#include "pch.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "GameObject.h"
#include <algorithm>
#include <stdexcept>

namespace Engine
{
    HealthComponent::HealthComponent(GameObject* gameObject, int maxHp, int armorBonus)
        : Component(gameObject), hp(maxHp), maxHp(maxHp), armor(armorBonus), maxArmor(armorBonus)
    {
        assert(gameObject != nullptr && "HealthComponent requires a valid GameObject");
        assert(maxHp > 0 && "HealthComponent: maxHp must be > 0");
        assert(armorBonus >= 0 && "HealthComponent: armorBonus cannot be negative");

        LOG_INFO("HealthComponent created for '" + gameObject->GetName()
            + "' HP=" + std::to_string(hp) + "/" + std::to_string(maxHp)
            + " ARMOR=" + std::to_string(armor) + "/" + std::to_string(maxArmor));
    }

    int HealthComponent::TakeDamage(int damage)
    {
        if (damage < 0) {
            LOG_ERROR("Negative damage (" + std::to_string(damage) + ") passed to "
                + GetGameObject()->GetName());
            throw std::invalid_argument("Damage cannot be negative!");
        }
        if (damage == 0) {
            LOG_WARN("Zero-damage hit ignored on " + GetGameObject()->GetName());
            return 0;
        }
        if (IsDead()) {
            LOG_WARN("TakeDamage ignored — " + GetGameObject()->GetName() + " is already dead");
            return 0;
        }

        const std::string name = GetGameObject()->GetName();

        if (armor > 0) {
            int absorbed = std::min(damage, armor);
            armor -= absorbed;
            damage -= absorbed;
            LOG_INFO(name + " absorbed " + std::to_string(absorbed)
                + " damage by armor. Armor left: " + std::to_string(armor) + "/"
                + std::to_string(maxArmor));
            if (armor == 0) LOG_WARN(name + " armor BROKEN!");
        }

        int hpTaken = 0;
        if (damage > 0) {
            hpTaken = std::min(damage, hp);
            hp -= hpTaken;
            LOG_INFO(name + " took " + std::to_string(hpTaken) + " HP damage. HP: "
                + std::to_string(hp) + "/" + std::to_string(maxHp));
        }

        if (IsDead()) {
            LOG_ERROR("Object '" + name + "' DESTROYED (HP reached 0)");
        }
        return hpTaken;
    }

    void HealthComponent::Heal(int amount)
    {
        if (amount < 0) {
            LOG_ERROR("Negative heal passed to " + GetGameObject()->GetName());
            throw std::invalid_argument("Heal cannot be negative!");
        }
        if (IsDead()) {
            LOG_WARN("Cannot heal dead object: " + GetGameObject()->GetName());
            return;
        }
        int before = hp;
        hp = std::min(maxHp, hp + amount);
        LOG_INFO(GetGameObject()->GetName() + " healed by " + std::to_string(hp - before)
            + ". HP: " + std::to_string(hp) + "/" + std::to_string(maxHp));
    }
}