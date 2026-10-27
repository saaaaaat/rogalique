#pragma once
#include "Component.h"

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    class ENGINE_API AttackComponent : public Component
    {
    public:
        AttackComponent(GameObject* gameObject) : Component(gameObject) {}
        void Update(float deltaTime) override;
        void Render() override {}
    private:
        float cooldown = 0.0f; 
    };
}
