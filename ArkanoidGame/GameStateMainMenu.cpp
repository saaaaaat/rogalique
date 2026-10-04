#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include <cassert>

namespace ArkanoidGame
{
	namespace
	{
		MenuItem CreateMenuItem(const std::string& label, const sf::Font& font,
			std::function<void(MenuItem&)> callback)
		{
			MenuItem item;
			item.text.setString(label);
			item.text.setFont(font);
			item.text.setCharacterSize(24);
			item.onPressCallback = callback;
			return item;
		}
	}

	void GameStateMainMenuData::Init()
	{
		assert(font.loadFromFile(SETTINGS.RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		MenuItem startGame = CreateMenuItem("Start Game", font, [](MenuItem&) {
			Application::Instance().GetGame().SwitchStateTo(GameStateType::Playing);
			});

		MenuItem records = CreateMenuItem("Records", font, [](MenuItem&) {
			Application::Instance().GetGame().PushState(GameStateType::Records, true);
			});

		MenuItem yes = CreateMenuItem("Yes", font, [](MenuItem&) {
			Application::Instance().GetGame().SwitchStateTo(GameStateType::None);
			});

		MenuItem no = CreateMenuItem("No", font, [this](MenuItem&) {
			menu.ReturnToParent();
			});

		MenuItem exitGame = CreateMenuItem("Exit Game", font, nullptr);
		exitGame.hintText.setString("Are you sure?");
		exitGame.hintText.setFont(font);
		exitGame.hintText.setCharacterSize(48);
		exitGame.hintText.setFillColor(sf::Color::Red);
		exitGame.childrenOrientation = Orientation::Horizontal;
		exitGame.childrenAlignment = Alignment::Middle;
		exitGame.childrenSpacing = 10.f;
		exitGame.childrens.push_back(yes);
		exitGame.childrens.push_back(no);

		MenuItem mainMenu;
		mainMenu.hintText.setString("Arkanoid Game");
		mainMenu.hintText.setFont(font);
		mainMenu.hintText.setCharacterSize(48);
		mainMenu.hintText.setFillColor(sf::Color::Red);
		mainMenu.childrenOrientation = Orientation::Vertical;
		mainMenu.childrenAlignment = Alignment::Middle;
		mainMenu.childrenSpacing = 10.f;
		mainMenu.childrens.push_back(startGame);
		mainMenu.childrens.push_back(records);
		mainMenu.childrens.push_back(exitGame);

		menu.Init(mainMenu);
	}

	void GameStateMainMenuData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				menu.ReturnToParent();
			}
			else if (event.key.code == sf::Keyboard::Enter)
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

	void GameStateMainMenuData::Update(float timeDelta)
	{
		
	}

	void GameStateMainMenuData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		sf::Text* hintText = &menu.GetActiveMenu().hintText;
		hintText->setOrigin(CalculateTextOrigin(*hintText, { 0.5f, 0.f }));
		hintText->setPosition(viewSize.x / 2.f, 150.f);
		window.draw(*hintText);

		menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}
}