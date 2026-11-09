#pragma once
#include "Component.h"
#include "Vector.h"

namespace RogaliqueGame
{
    class ExploderComponent : public Engine::Component
    {
    public:
        static constexpr float TRIGGER_RADIUS = 120.f;
        static constexpr float EXPLODE_RADIUS = 110.f;
        static constexpr int   EXPLODE_DAMAGE = 80;
        static constexpr float COUNTDOWN = 2.f;

        ExploderComponent(Engine::GameObject* obj);

        void Update(float dt) override;
        void Render() override {}

    private:
        bool armed = false;
        float countdown = 0.f;
        void Explode();
    };
}