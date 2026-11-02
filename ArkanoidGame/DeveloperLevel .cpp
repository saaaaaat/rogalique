#include "DeveloperLevel.h"
#include "Application.h" 
#include "Game.h"    
#include "GameWorld.h" 

#include "TransformComponent.h"
#include <cmath>
#include <iostream>
#include <SFML/Window/Keyboard.hpp> 

using namespace Engine;

namespace RogaliqueGame
{
    void DeveloperLevel::Start()
    {
        player = std::make_shared<Player>();
        enemy = std::make_shared<Enemy>();

        float w = 1280.f;
        float h = 720.f;
        float t = 40.f;

        walls.push_back(std::make_shared<Wall>(sf::Vector2f(w / 2, t / 2), sf::Vector2f(w, t)));
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(w / 2, h - t / 2), sf::Vector2f(w, t)));
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(t / 2, h / 2), sf::Vector2f(t, h)));
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(w - t / 2, h / 2), sf::Vector2f(t, h)));

        walls.push_back(std::make_shared<Wall>(sf::Vector2f(300, 360), sf::Vector2f(40, 400)));
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(980, 360), sf::Vector2f(40, 400)));
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(640, 200), sf::Vector2f(300, 40)));
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(640, 520), sf::Vector2f(300, 40)));
    }

    void DeveloperLevel::Update(float timeDelta)
    {
       
        if (::Engine::GameWorld::Instance()->IsPaused())
        {
          
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                std::cout << "Restarting game..." << std::endl;
                ::Engine::GameWorld::Instance()->SetPaused(false);
                Restart();
            }
            return; 
        }

        if (!player || !enemy) return;

        auto pTransform = player->GetGameObject()->GetComponent<TransformComponent>();
        auto eTransform = enemy->GetGameObject()->GetComponent<TransformComponent>();

        if (pTransform && eTransform)
        {
            Vector2Df pPos = pTransform->GetWorldPosition();
            Vector2Df ePos = eTransform->GetWorldPosition();

            float dist = std::sqrt(std::pow(pPos.x - ePos.x, 2) + std::pow(pPos.y - ePos.y, 2));

            if (dist < 32.f)
            {
                std::cout << " GAME OVER " << std::endl;
                ::Engine::GameWorld::Instance()->SetPaused(true);
            }
        }
    }

    void DeveloperLevel::Restart()
    {
        Stop();
        Start();
    }

    void DeveloperLevel::Stop()
    {
        ::Engine::GameWorld::Instance()->Clear();
        walls.clear();
    }
}