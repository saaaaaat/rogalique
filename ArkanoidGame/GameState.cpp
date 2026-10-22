#include "GameState.h"
#include "GameStateData.h"
#include "GameStatePlaying.h"



#include "GameStateMainMenu.h"

#include <cassert>

namespace RogaliqueGame
{
	GameState::GameState(GameStateType type, bool isExclusivelyVisible)
		: type(type)
		, isExclusivelyVisible(isExclusivelyVisible)
	{
		switch (type)
		{
		case GameStateType::MainMenu:
			data = std::make_shared<GameStateMainMenuData>();
			break;
		case GameStateType::Playing:
			data = std::make_shared<GameStatePlayingData>();
			break;
	
		default:
			assert(false);
			break;
		}

		if (data)
		{
			data->Init();
		}
	}

	GameState::~GameState()
	{
		if (data) {
			data = nullptr;
		}
		
	}

	

	void GameState::Update(float timeDelta)
	{
		data->Update(timeDelta);
	}

	void GameState::Draw(sf::RenderWindow& window)
	{
		data->Draw(window);
	}

	void GameState::HandleWindowEvent(const sf::Event& event)
	{
		data->HandleWindowEvent(event);
	}
}