#include "GameState.h"
#include "GameStateData.h"
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStateWin.h"    
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"
#include <cassert>

namespace ArkanoidGame
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
		case GameStateType::GameOver:
			data = std::make_shared<GameStateGameOverData>();
			break;
		case GameStateType::GameWin:
			data = std::make_shared<GameStateWinData>();
			break;
		case GameStateType::Records:     
			data = std::make_shared<GameStateRecordsData>();
			break;
		case GameStateType::ExitDialog:
			data = std::make_shared<GameStatePauseMenuData>();
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