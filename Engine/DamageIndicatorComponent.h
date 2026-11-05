#pragma once
#include "Component.h"
#include "Vector.h"
#include <SFML/Graphics.hpp>
#include <string>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

namespace Engine
{
    class ENGINE_API DamageIndicatorComponent : public Component
    {
    public:
        DamageIndicatorComponent(GameObject* gameObject,
            const std::string& text,
            sf::Color color,
            float lifetime = 1.2f);

        void Update(float deltaTime) override;
        void Render() override;

    private:
        sf::Text textObject;
        Vector2Df velocity = { 0.f, 40.f }; 
        float lifetime;
        float elapsed = 0.f;
    };
}