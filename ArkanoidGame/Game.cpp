#include "Game.h"
#include <cassert>
#include <algorithm>
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"

namespace ArkanoidGame
{
	Game::Game()
	{

		recordsTable =
		{
			{"John", SETTINGS.MAX_APPLES / 2},
			{"Jane", SETTINGS.MAX_APPLES / 3},
			{"Alice", SETTINGS.MAX_APPLES / 4},
			{"Bob", SETTINGS.MAX_APPLES / 5},
			{SETTINGS.PLAYER_NAME,0},
		};
		//иниц.состояния

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		SwitchStateTo(GameStateType::MainMenu);
	}

	Game::~Game()
	{
		Shutdown();
	}

	void Game::HandleWindowEvents(sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}



			if (!stateStack.empty())
			{
				stateStack.back().HandleWindowEvent(event);
			}
		}
	}

	bool Game::Update(float timeDelta)
	{
		if (stateChangeType == GameStateChangeType::Switch)
		{
			stateStack.clear();
		}
		else if (stateChangeType == GameStateChangeType::Pop)
		{
			if (!stateStack.empty())
			{
				stateStack.pop_back();
			}
		}

		if (pendingGameStateType != GameStateType::None)
		{
			stateStack.push_back(GameState(pendingGameStateType, pendingGameStateIsExclusivelyVisible));
		}

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;

		if (!stateStack.empty())
		{
			stateStack.back().Update(timeDelta);
			return true;
		}

		return false;
	}

	void Game::Draw(sf::RenderWindow& window)
	{
		if (!stateStack.empty())
		{
			std::vector<GameState*> visible;
			for (auto it = stateStack.rbegin(); it != stateStack.rend(); ++it)
			{
				visible.push_back(&(*it));
				if (it->IsExclusivelyVisible()) break;
			}

			for (auto it = visible.rbegin(); it != visible.rend(); ++it)
			{
				(*it)->Draw(window);
			}
		}
	}

	void Game::Shutdown()
	{
		stateStack.clear();

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
	}

	void Game::PushState(GameStateType stateType, bool isExclusivelyVisible)
	{
		pendingGameStateType = stateType;
		pendingGameStateIsExclusivelyVisible = isExclusivelyVisible;
		stateChangeType = GameStateChangeType::Push;
	}

	void Game::PopState()
	{
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		stateChangeType = GameStateChangeType::Pop;
	}

	void Game::SwitchStateTo(GameStateType newState)
	{
		pendingGameStateType = newState;
		pendingGameStateIsExclusivelyVisible = false;
		stateChangeType = GameStateChangeType::Switch;
	}

	bool Game::IsEnableOptions(GameOptions option) const
	{
		return ((std::uint8_t)options & (std::uint8_t)option) != (std::uint8_t)GameOptions::Empty;
	}

	void Game::SetOption(GameOptions option, bool value)
	{
		if (value)
		{
			options = (GameOptions)((std::uint8_t)options | (std::uint8_t)option);
		}
		else
		{
			options = (GameOptions)((std::uint8_t)options & ~(std::uint8_t)option);
		}
	}

	void Game::StartGame()
	{
		SwitchStateTo(GameStateType::Playing);
	}

	void Game::PauseGame()
	{
		PushState(GameStateType::ExitDialog, false);
	}

	void Game::WinGame()
	{
		PushState(GameStateType::GameWin, false);
	}

	void Game::LooseGame()
	{
		PushState(GameStateType::GameOver, false);
	}

	void Game::ExitGame()
	{
		SwitchStateTo(GameStateType::MainMenu);
	}

	void Game::QuitGame()
	{
		SwitchStateTo(GameStateType::None);
	}

	void Game::ShowRecords()
	{
		PushState(GameStateType::Records, true);
	}

	void Game::LoadNextLevel()
	{
		assert(stateStack.back().GetType() == GameStateType::Playing);
		auto playingData = stateStack.back().GetData<GameStatePlayingData>();
		playingData->LoadNextLevel();
	}
	void Game::UpdateGame(float timeDelta, sf::RenderWindow& window)
	{
		HandleWindowEvents(window);
		if (Update(timeDelta))
		{
			window.clear();
			Draw(window);
			window.display();
		}
		else
		{
			window.close();
		}
	}

	int Game::GetRecordByPlayerId(const std::string & playerId) const
	{
		auto it = recordsTable.find(playerId);
		return it == recordsTable.end() ? 0 : it->second;
	}

	void Game::UpdateRecord(const std::string & playerId, int score)
	{
		recordsTable[playerId] = std::max(recordsTable[playerId], score);
	}
}

