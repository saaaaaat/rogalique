#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "DeveloperLevel.h"
#include "RenderSystem.h"

namespace RogaliqueGame
{
	void GameStatePlayingData::Init()
	{
		// создаем и запускаем уровень
		level = std::make_shared<DeveloperLevel>();
		level->Start();
	}

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed &&
			event.key.code == sf::Keyboard::Escape)
		{
		
			Application::Instance().GetGame().ExitGame();
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{
		
		if (level)
			level->Update(timeDelta);

		
		Engine::GameWorld::Instance()->Update(timeDelta);
	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		
		window.clear(sf::Color(30, 30, 30));

		// рисуем все объекты
		Engine::GameWorld::Instance()->Render();
	}
}