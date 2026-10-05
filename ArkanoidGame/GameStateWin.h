#pragma once
#include "SFML/Graphics.hpp"
#include "GameStateData.h"
#include "Menu.h"

namespace ArkanoidGame
{
	class Game;

	class GameStateWinData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

	private:
		sf::Font font;
		std::vector<sf::Text> recordsTableTexts;
		sf::RectangleShape background;
		sf::Text winText;
		sf::Text hintText;
		sf::Text scoreText;
		Menu menu;
	};
}
