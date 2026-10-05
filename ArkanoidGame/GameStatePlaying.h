#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameStateData.h"
#include "Platform.h"
#include "Ball.h"
#include "LevelLoader.h"
#include "BlockFactory.h"
#include "IObserver.h"
#include "Bonus.h"
#include "BigPlatformBonus.h"
#include"FireBallBonus.h"
#include"SlowBallBonus.h"
#include <unordered_map>

namespace ArkanoidGame
{
	class Game;
	class Block;
	class BlockFactory;

	class GameStatePlayingData : public GameStateData, public IObserver, public std::enable_shared_from_this<GameStatePlayingData>
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;
		void LoadNextLevel();
		void Notify(std::shared_ptr<IObservable> observable) override;
		void ApplyBonus(BonusType type);
		void ResetEffects();

	private:
		void createBlocks();
		void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect,
			bool& needInverseDirX, bool& needInverseDirY);

		sf::Font font;
		sf::SoundBuffer gameOverSoundBuffer;

		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::vector<std::shared_ptr<Block>> blocks;

		std::vector<std::shared_ptr<Bonus>> bonuses;

		// Эффекты
		float bonusTimer = 0.f;        // таймер эффекта
		bool hasActiveEffect = false;
		BonusType activeBonusType;

		sf::Text scoreText;
		sf::Text livesText;
		sf::RectangleShape background;

		sf::Sound gameOverSound;

		std::unordered_map<BlockType, std::unique_ptr<BlockFactory>> factories;
		int breackableBlocksCount = 0;

		bool shouldLoadNextLevel = false;

		// уровни
		LevelLoader levelLoader;
		int currentLevel = 0;

		int lives = 3;
		

		int score = 0;

	};
}