#pragma once
#include "Component.h"
#include "Vector.h"

namespace RogaliqueGame
{
    class ChaseAIComponent : public Engine::Component
    {
    public:
        static constexpr float SPEED = 140.f;
        static constexpr float RADIUS = 14.f;
        static constexpr float CONTACT_RADIUS = 36.f;
        static constexpr float CONTACT_INTERVAL = 1.f;
        static constexpr int   CONTACT_DAMAGE = 20;

        ChaseAIComponent(Engine::GameObject* obj);

        void Update(float dt) override;
        void Render() override {}

        void Stun(float duration) { stunTimer = (stunTimer > duration) ? stunTimer : duration; }
        bool IsStunned() const { return stunTimer > 0.f; }
        float GetStunTimer() const { return stunTimer; }

    private:
        float stunTimer = 0.f;
        float contactTimer = 0.f;
    };
}
