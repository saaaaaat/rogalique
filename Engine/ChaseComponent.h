#pragma once
#include "Component.h"

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine {
    class ENGINE_API ChaseComponent : public Component {
    public:
        ChaseComponent(GameObject* gameObject) : Component(gameObject) {}
        void Update(float deltaTime) override;
        void Render() override {}
    };
}