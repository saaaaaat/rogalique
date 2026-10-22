#include "DeveloperLevel.h"
#include "Application.h" 
#include "Game.h"    
#include "GameWorld.h" 
#include <cmath>

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

        
        // верхняя граница
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(w / 2, t / 2), sf::Vector2f(w, t)));
        // нижняя
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(w / 2, h - t / 2), sf::Vector2f(w, t)));
        // левая
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(t / 2, h / 2), sf::Vector2f(t, h)));
        // правая
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(w - t / 2, h / 2), sf::Vector2f(t, h)));

        // лабиринт
        
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(300, 360), sf::Vector2f(40, 400)));
        
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(980, 360), sf::Vector2f(40, 400)));

        walls.push_back(std::make_shared<Wall>(sf::Vector2f(640, 200), sf::Vector2f(300, 40)));

        walls.push_back(std::make_shared<Wall>(sf::Vector2f(640, 520), sf::Vector2f(300, 40)));
    }

    void DeveloperLevel::Update(float timeDelta)
    {
        auto playerTransform = player->GetGameObject()->GetComponent<TransformComponent>();
        auto enemyTransform = enemy->GetGameObject()->GetComponent<TransformComponent>();

        if (playerTransform && enemyTransform)
        {
            Vector2Df pPos = playerTransform->GetWorldPosition();
            Vector2Df ePos = enemyTransform->GetWorldPosition();

      
            float dist = std::sqrt(std::pow(pPos.x - ePos.x, 2) + std::pow(pPos.y - ePos.y, 2));

         //не работает
            if (dist < 32.f)
            {
                
                Application::Instance().GetGame().LooseGame();

             
                playerTransform->SetWorldPosition(player->startPosition.x, player->startPosition.y);
                enemyTransform->SetWorldPosition(enemy->startPosition.x, enemy->startPosition.y);

             
                player->GetGameObject()->GetComponent<RigidbodyComponent>()->SetLinearVelocity({ 0, 0 });
                enemy->GetGameObject()->GetComponent<RigidbodyComponent>()->SetLinearVelocity({ 0, 0 });
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
    }
}