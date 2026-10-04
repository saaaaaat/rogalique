#include "GameStateWin.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <cassert>

namespace ArkanoidGame
{
	void GameStateWinData::Init()
	{
		assert(font.loadFromFile(SETTINGS.RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		sf::Color bg = sf::Color::Black;
		bg.a = 200;
		background.setFillColor(bg);

		// Победа
		winText.setFont(font);
		winText.setCharacterSize(48);
		winText.setStyle(sf::Text::Bold);
		winText.setFillColor(sf::Color::Green);
		winText.setString("YOU WIN");

		int playerScore = Application::Instance().GetGame().GetRecordByPlayerId(SETTINGS.PLAYER_NAME);
		scoreText.setFont(font);
		scoreText.setCharacterSize(32);
		scoreText.setFillColor(sf::Color::Yellow);
		scoreText.setString("Your score: " + std::to_string(playerScore));

		// Подсказка
		hintText.setFont(font);
		hintText.setCharacterSize(24);
		hintText.setFillColor(sf::Color::White);
		hintText.setString("Play again?");

		// Меню Да/Нет
		MenuItem yesItem;
		yesItem.text.setString("Yes");
		yesItem.text.setFont(font);
		yesItem.text.setCharacterSize(24);
		yesItem.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().StartGame();
			};

		MenuItem noItem;
		noItem.text.setString("No");
		noItem.text.setFont(font);
		noItem.text.setCharacterSize(24);
		noItem.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().ExitGame();
			};

		MenuItem winMenu;
		winMenu.childrenOrientation = Orientation::Horizontal;
		winMenu.childrenAlignment = Alignment::Middle;
		winMenu.childrenSpacing = 30.f;
		winMenu.childrens.push_back(yesItem);
		winMenu.childrens.push_back(noItem);

		menu.Init(winMenu);  
	}

	void GameStateWinData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Enter)
			{
				menu.ActivateSelectedItem();   
			}

			Orientation orientation = menu.GetActiveMenu().childrenOrientation;  
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				menu.SelectPreviousItem();   
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				menu.SelectNextItem();   
			}
		}
	}

	void GameStateWinData::Update(float timeDelta)
	{
		
	}

	void GameStateWinData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		background.setOrigin(0.f, 0.f);
		background.setSize(viewSize);
		window.draw(background);

		winText.setOrigin(CalculateTextOrigin(winText, { 0.5f, 0.5f }));   
		winText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 150.f);
		window.draw(winText);

		scoreText.setOrigin(CalculateTextOrigin(scoreText, { 0.5f, 0.5f }));
		scoreText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 70.f);
		window.draw(scoreText);

		hintText.setOrigin(CalculateTextOrigin(hintText, { 0.5f, 0.5f }));  
		hintText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 20.f);
		window.draw(hintText);

		menu.Draw(window, { viewSize.x / 2.f, viewSize.y / 2.f + 50.f }, { 0.5f, 0.f });
	}
}