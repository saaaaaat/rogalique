#pragma once
#include "Component.h"

namespace RogaliqueGame
{
    class ExitComponent : public Engine::Component
    {
    public:
        static constexpr float TRIGGER_RADIUS = 36.f;

        ExitComponent(Engine::GameObject* obj);
        void Update(float dt) override;
        void Render() override {}
    };
}