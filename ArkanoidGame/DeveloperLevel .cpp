#include "pch.h"
#include "DeveloperLevel.h"
#include "Application.h" 
#include "Game.h"    
#include "GameWorld.h" 
#include "Logger.h"
#include "HealthComponent.h" // <--- Важно для проверки здоровья
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    void DeveloperLevel::Start()
    {
        LOG_INFO("Starting level DeveloperLevel");

        auto floor = std::make_shared<Floor>(sf::Vector2f(640.f, 360.f), sf::Vector2f(1280.f, 720.f));

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

        LOG_INFO("--- Level loaded successfully! ---");
    }

    void DeveloperLevel::Update(float timeDelta)
    {
        if (player)
        {
            auto playerHealth = player->GetGameObject()->GetComponent<HealthComponent>();

            // Если игрок умер — перезапускаем игру
            if (playerHealth && playerHealth->IsDead())
            {
                LOG_INFO("Player is DEAD! Restarting game...");
                Restart();
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
        Engine::GameWorld::Instance()->Clear();
    }
}