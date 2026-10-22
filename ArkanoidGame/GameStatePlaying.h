#pragma once
#include "SFML/Graphics.hpp"
#include "GameStateData.h"
#include "DeveloperLevel.h" 
#include <memory>

namespace RogaliqueGame
{
	class GameStatePlayingData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

	private:
		std::shared_ptr<DeveloperLevel> level; 
	};
}