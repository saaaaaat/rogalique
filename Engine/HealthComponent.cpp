#include "pch.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "GameObject.h"
#include <algorithm>

namespace Engine
{
    HealthComponent::HealthComponent(GameObject* gameObject, int maxHp, int armorBonus)
        : Component(gameObject), hp(maxHp), maxHp(maxHp), armor(armorBonus), maxArmor(armorBonus)
    {
        LOG_INFO("Создан HealthComponent для " + gameObject->GetName() + ". HP: " + std::to_string(hp) + ", Броня: " + std::to_string(armor));
    }

    void HealthComponent::TakeDamage(int damage)
    {
        if (damage <= 0) return;

        // Сначала урон забирает броня
        if (armor > 0) {
            int armorDamage = std::min(damage, armor);
            armor -= armorDamage;
            damage -= armorDamage;
            LOG_WARN("Броня поглотила " + std::to_string(armorDamage) + " урона. Осталось брони: " + std::to_string(armor));
        }

        // Если урон еще остался, он бьет по здоровью
        if (damage > 0) {
            hp -= damage;
            LOG_ERROR("Нанесено " + std::to_string(damage) + " урона в HP! Осталось HP: " + std::to_string(hp));
        }

        if (hp <= 0) {
            hp = 0;
            LOG_ERROR("Объект " + GetGameObject()->GetName() + " УНИЧТОЖЕН!");
        }
    }
}