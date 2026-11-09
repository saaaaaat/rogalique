#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include <SFML/Graphics.hpp>

namespace RogaliqueGame
{
    class CircleRendererComponent : public Engine::Component
    {
    public:
        CircleRendererComponent(Engine::GameObject* obj, float radius, sf::Color color,
            sf::Color outline = sf::Color::Black, float outlineThickness = 2.f)
            : Component(obj), radius(radius)
        {
            shape.setRadius(radius);
            shape.setOrigin(radius, radius);
            shape.setFillColor(color);
            shape.setOutlineColor(outline);
            shape.setOutlineThickness(outlineThickness);
        }

        void Update(float) override {}
        void Render() override
        {
            auto* t = GetGameObject()->GetComponent<Engine::TransformComponent>();
            if (!t) return;
            auto p = t->GetWorldPosition();
            shape.setPosition(p.x, p.y);
            Engine::RenderSystem::Instance()->Render(shape);
        }

        void SetColor(sf::Color c) { shape.setFillColor(c); }
        sf::Color GetColor() const { return shape.getFillColor(); }
        float GetRadius() const { return radius; }

    private:
        float radius;
        sf::CircleShape shape;
    };
}
