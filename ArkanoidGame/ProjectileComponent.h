#pragma once
#include "Component.h"
#include "../Engine/Vector.h"
namespace RogaliqueGame {
    class ProjectileComponent : public Engine::Component {
    public:
        ProjectileComponent(Engine::GameObject* obj, Engine::Vector2Df velocity, float lifetime = 5.f);
        void Update(float dt) override;
        void Render() override {}
    private:
        Engine::Vector2Df velocity;
        float lifetime;
    };
}