#include "GameStateGameOver.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>
#include <sstream>
#include <algorithm>


namespace ArkanoidGame
{
	const char* PLAYER_NAME = "Player";

	void GameStateGameOverData::Init()
	{
		assert(font.loadFromFile(SETTINGS.RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		timeSinceGameOver = 0.f;

		sf::Color bg = sf::Color::Black;
		bg.a = 200;
		background.setFillColor(bg);

		gameOverText.setFont(font);
		gameOverText.setCharacterSize(48);
		gameOverText.setStyle(sf::Text::Bold);
		gameOverText.setFillColor(sf::Color::Red);
		gameOverText.setString("GAME OVER");

		recordsTableTexts.reserve(SETTINGS.MAX_RECORDS_TABLE_SIZE);

		 //собираем  вектор ,сортируем
		std::vector<std::pair<int, std::string>> sorted;
		Game& game = Application::Instance().GetGame();
		for (const auto& item : game.GetRecordsTable())
		{
			sorted.push_back({ item.second, item.first });
		}

		std::sort(sorted.begin(), sorted.end(),
			[](const auto& a, const auto& b) { return a.first > b.first; });

		bool playerFound = false;
		for (int i = 0; i < SETTINGS.MAX_RECORDS_TABLE_SIZE && i < (int)sorted.size(); ++i)
		{
			recordsTableTexts.emplace_back();
			sf::Text& text = recordsTableTexts.back();

			std::stringstream ss;
			ss << i + 1 << ". " << sorted[i].second << ": " << sorted[i].first;
			text.setString(ss.str());
			text.setFont(font);
			text.setCharacterSize(24);

			if (sorted[i].second == PLAYER_NAME)
			{
				text.setFillColor(sf::Color::Green);
				playerFound = true;
			}
			else
			{
				text.setFillColor(sf::Color::White);
			}
		}

		if (!playerFound && !recordsTableTexts.empty())
		{
			sf::Text& last = recordsTableTexts.back();
			std::stringstream ss;
			int score = game.GetRecordByPlayerId(PLAYER_NAME);
			ss << SETTINGS.MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << score;
			last.setString(ss.str());
			last.setFillColor(sf::Color::Green);
		}

		hintText.setFont(font);
		hintText.setCharacterSize(24);
		hintText.setFillColor(sf::Color::White);
		hintText.setString("Press R to restart\nM to exit to main menu");
	}

	void GameStateGameOverData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::R)
			{
				Application::Instance().GetGame().SwitchStateTo(GameStateType::Playing);
			}
			else if (event.key.code == sf::Keyboard::M)
			{
				Application::Instance().GetGame().SwitchStateTo(GameStateType::MainMenu);
			}
		}
	}

	void GameStateGameOverData::Update(float timeDelta)
	{
		
	}

	void GameStateGameOverData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		background.setOrigin(0.f, 0.f);
		background.setSize(viewSize);
		window.draw(background);

		gameOverText.setOrigin(CalculateTextOrigin(gameOverText, { 0.5f, 1.f }));
		gameOverText.setPosition(viewSize.x / 2.f, viewSize.y / 2 - 50.f);
		window.draw(gameOverText);

		std::vector<sf::Text*> textsList;
		for (auto& text : recordsTableTexts)
		{
			textsList.push_back(&text);
		}

		sf::Vector2f tablePos = { viewSize.x / 2, viewSize.y / 2.f +30.f };
		DrawTextArray(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePos, { 0.5f, 0.f });

		hintText.setOrigin(CalculateTextOrigin(hintText, { 0.5f, 1.f }));
		hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText);
	}
}