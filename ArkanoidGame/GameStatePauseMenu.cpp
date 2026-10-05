#include "GameStatePauseMenu.h"
#include "Application.h"
#include "Game.h"
#include <cassert>

namespace ArkanoidGame
{
	void GameStatePauseMenuData::Init()
	{
		assert(font.loadFromFile(SETTINGS.RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		background.setFillColor(sf::Color(0, 0, 0, 128));

		titleText.setString("Pause");
		titleText.setFont(font);
		titleText.setCharacterSize(48);
		titleText.setFillColor(sf::Color::Red);

		MenuItem resume;
		resume.text.setString("Return to game");
		resume.text.setFont(font);
		resume.text.setCharacterSize(24);
		resume.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().PopState();
			};

		MenuItem exit;
		exit.text.setString("Exit to main menu");
		exit.text.setFont(font);
		exit.text.setCharacterSize(24);
		exit.onPressCallback = [](MenuItem&) {
			Application::Instance().GetGame().SwitchStateTo(GameStateType::MainMenu);
			};

		MenuItem pauseMenu;
		pauseMenu.childrenOrientation = Orientation::Vertical;
		pauseMenu.childrenAlignment = Alignment::Middle;
		pauseMenu.childrens.push_back(resume);
		pauseMenu.childrens.push_back(exit);

		menu.Init(pauseMenu);
	}

	void GameStatePauseMenuData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PopState();
			}

			if (event.key.code == sf::Keyboard::Enter)
			{
				menu.ActivateSelectedItem();
			}

			if (event.key.code == sf::Keyboard::Up)
			{
				menu.SelectPreviousItem();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				menu.SelectNextItem();
			}
		}
	}

	void GameStatePauseMenuData::Update(float timeDelta)
	{
		
	}

	void GameStatePauseMenuData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		background.setSize(viewSize);
		window.draw(background);

		titleText.setOrigin(CalculateTextOrigin(titleText, { 0.5f, 0.f }));
		titleText.setPosition(viewSize.x / 2.f, 100.f);
		window.draw(titleText);

		menu.Draw(window, window.getView().getCenter(), { 0.5f, 0.f });
	}
}