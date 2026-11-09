#pragma once
#include "Component.h"

namespace RogaliqueGame
{
    enum class ItemType { Clone, Sword, Speed, Key };

    const char* ItemTypeToString(ItemType t);

    class PickupComponent : public Engine::Component
    {
    public:
        static constexpr float PICKUP_RADIUS = 32.f;

        PickupComponent(Engine::GameObject* obj, ItemType type);

        void Update(float dt) override;
        void Render() override {}

    private:
        ItemType type;
        float warnTimer = 0.f;
    };
}