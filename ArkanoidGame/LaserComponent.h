#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

namespace RogaliqueGame {

    class LaserComponent : public Engine::Component
    {
    public:
        LaserComponent(Engine::GameObject* obj,
            sf::Vector2f center, sf::Vector2f size,
            float angleDeg = 0.f,
            float warningTime = 0.9f,
            float activeTime = 0.4f,
            int damage = 30);
        void Update(float dt) override;
        void Render() override;

    private:
        enum class State { Warning, Active, Done };
        State state = State::Warning;
        float timer = 0.f;
        float warningTime;
        float activeTime;
        int damage;
        bool hasDamagedPlayer = false;
        sf::RectangleShape shape;
        sf::Vector2f center;
        sf::Vector2f size;
        float angle;

        bool ContainsPoint(float px, float py) const;
    };
}