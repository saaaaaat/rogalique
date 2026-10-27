#include "DeveloperLevel.h"
#include "Application.h" 
#include "Game.h"    
#include "GameWorld.h" 
#include "Logger.h"
#include "HealthComponent.h"
#include <cmath>

using namespace Engine;

namespace RogaliqueGame
{
    void DeveloperLevel::Start()
    {
        LOG_INFO("Запуск уровня DeveloperLevel");

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

        LOG_INFO("Уровень успешно загружен!");
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

            // Если они близко (поставим 100, чтобы наверняка)
            if (dist < 100.f)
            {
                // 1. Орем в консоль напрямую, без логгера
                std::cout << "!!! РАССТОЯНИЕ: " << dist << " (БЛИЗКО) !!!" << std::endl;

                // 2. Проверяем, видим ли мы компонент здоровья
                auto playerHealth = player->GetGameObject()->GetComponent<HealthComponent>();
                auto enemyHealth = enemy->GetGameObject()->GetComponent<HealthComponent>();

                if (playerHealth == nullptr) {
                    std::cout << "ОШИБКА: У ИГРОКА НЕТ HEALTHCOMPONENT!" << std::endl;
                }
                if (enemyHealth == nullptr) {
                    std::cout << "ОШИБКА: У ВРАГА НЕТ HEALTHCOMPONENT!" << std::endl;
                }

                if (playerHealth && enemyHealth)
                {
                    LOG_INFO("Атака! Наносим урон.");
                    playerHealth->TakeDamage(15);
                    enemyHealth->TakeDamage(5);

                    // Откидываем на старт
                    playerTransform->SetWorldPosition(player->startPosition.x, player->startPosition.y);
                    enemyTransform->SetWorldPosition(enemy->startPosition.x, enemyTransform->GetWorldPosition().y);
                }
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