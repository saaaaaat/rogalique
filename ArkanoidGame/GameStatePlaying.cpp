#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Logger.h"
#include "GameWorld.h" // <--- Важно для Engine::GameWorld
#include <iostream>

namespace RogaliqueGame
{
	void GameStatePlayingData::Init()
	{
		LOG_INFO("--- GameStatePlaying Init ---");

		// Очищаем мир от старых объектов перед созданием уровня
		Engine::GameWorld::Instance()->Clear();

		level = std::make_shared<DeveloperLevel>();
		level->Start();
	}

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			Application::Instance().GetGame().ExitGame();
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{
		// 1. Обновляем логику уровня (если она есть)
		if (level)
		{
			level->Update(timeDelta);
		}

		// 2. Обновляем движок (здесь работают все компоненты: физика, AttackComponent, HealthComponent)
		Engine::GameWorld::Instance()->Update(timeDelta);
	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		window.clear(sf::Color(30, 30, 30));
		Engine::GameWorld::Instance()->Render();
	}
}