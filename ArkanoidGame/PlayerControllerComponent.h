#pragma once
#include "Component.h"
#include "Vector.h"

namespace RogaliqueGame
{
    class PlayerControllerComponent : public Engine::Component
    {
    public:
        static constexpr float WALK_SPEED = 150.f;
        static constexpr float SPRINT_SPEED = 260.f;
        static constexpr float PLAYER_RADIUS = 14.f;
        static constexpr float STAMINA_MAX = 100.f;
        static constexpr float STAMINA_DRAIN = 20.f;
        static constexpr float STAMINA_REGEN = 10.f;

        PlayerControllerComponent(Engine::GameObject* obj);

        void Update(float deltaTime) override;
        void Render() override {}

        float GetStamina() const { return stamina; }
        float GetStaminaMax() const { return STAMINA_MAX; }

    private:
        float stamina = STAMINA_MAX;
        float logTimer = 0.f;
        int   lastBlock = 0;
    };
}
