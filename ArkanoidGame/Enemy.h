#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp> 

namespace RogaliqueGame {
    class Enemy {
    public:
        Enemy();
        Engine::GameObject* GetGameObject() { return gameObject; }
        sf::Vector2f startPosition; 
    private:
        Engine::GameObject* gameObject;
    };
}