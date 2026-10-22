#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

namespace RogaliqueGame {
    class Wall {
    public:
        Wall(sf::Vector2f position, sf::Vector2f size);
    private:
        Engine::GameObject* gameObject;
    };
}
