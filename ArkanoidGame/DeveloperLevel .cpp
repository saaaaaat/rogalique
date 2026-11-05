#include "pch.h"
#include "DeveloperLevel.h"
#include "Application.h" 
#include "Game.h"    
#include "GameWorld.h" 
#include "Logger.h"
#include "HealthComponent.h" 
#include "MazeGenerator.h"
#include <cmath>
#include <cassert>
#include "TransformComponent.h"
#include <iostream>
#include <SFML/Window/Keyboard.hpp> 


using namespace Engine;

namespace RogaliqueGame
{
    void DeveloperLevel::Start()
    {
        LOG_INFO("Starting level DeveloperLevel");

        int width = 30;
        int height = 20;
        float cellSize = 128.f; 

        MazeGenerator mazeGenerator(width, height, this);
        mazeGenerator.Generate();

    
        player = std::make_shared<Player>();
        auto pTransform = player->GetGameObject()->GetComponent<Engine::TransformComponent>();
        if (pTransform) {
          
            pTransform->SetWorldPosition(cellSize, cellSize);
        }

       
        enemy = std::make_shared<Enemy>();
        auto eTransform = enemy->GetGameObject()->GetComponent<Engine::TransformComponent>();
        if (eTransform) {
            eTransform->SetWorldPosition(cellSize * 5, cellSize * 5);
        }

        LOG_INFO("--- Level loaded successfully! ---");
    }


    void DeveloperLevel::Update(float timeDelta)
    {

        if (::Engine::GameWorld::Instance()->IsPaused())
        {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                std::cout << "Restarting game..." << std::endl;
                Restart();
            }
            return;
        }


        if (!player || !enemy) return;


        auto playerHealth = player->GetGameObject()->GetComponent<HealthComponent>();
        if (playerHealth && playerHealth->IsDead())
        {
            LOG_WARN("Player is DEAD! GAME OVER");

            ::Engine::GameWorld::Instance()->SetPaused(true);
            return;
        }


        auto enemyHealth = enemy->GetGameObject()->GetComponent<HealthComponent>();
        if (enemyHealth && enemyHealth->IsDead())
        {
            LOG_WARN("Enemy is DEAD! YOU WIN!");

            ::Engine::GameWorld::Instance()->SetPaused(true);
            return;
        }

    }

    void DeveloperLevel::Restart()
    {
        LOG_INFO("Restarting level...");
        Stop();
        Start();
    }

    void DeveloperLevel::Stop()
    {

        walls.clear();
        floors.clear();


        player.reset();
        enemy.reset();

        ::Engine::GameWorld::Instance()->Clear();
        ::Engine::GameWorld::Instance()->SetPaused(false);
    }
}