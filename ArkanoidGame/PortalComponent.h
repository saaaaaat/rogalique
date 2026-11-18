#pragma once
#include "Component.h"
namespace RogaliqueGame {
    class PortalComponent : public Engine::Component {
    public:
        PortalComponent(Engine::GameObject* obj);
        void Update(float dt) override;
        void Render() override {}
    };
}