#include "GameStatePlaying.h"
#include "Application.h"
#include "Block.h"
#include "Game.h"
#include "Text.h"
#include "Bonus.h"
#include "FireBallBonus.h"
#include "SlowBallBonus.h"
#include "BigPlatformBonus.h"

#include <cassert>
#include <algorithm>

namespace ArkanoidGame
{
	void GameStatePlayingData::Init()
	{
		assert(font.loadFromFile(SETTINGS.FONTS_PATH + "Roboto-Regular.ttf"));
		assert(gameOverSoundBuffer.loadFromFile(SETTINGS.SOUNDS_PATH + "Death.wav"));

		factories.emplace(BlockType::Simple, std::make_unique<SimpleBlockFactory>());
		factories.emplace(BlockType::ThreeHit, std::make_unique<ThreeHitBlockFactory>());
		factories.emplace(BlockType::Unbreackable, std::make_unique<UnbreackableBlockFactory>());
		factories.emplace(BlockType::Glass, std::make_unique<GlassBlockFactory>());

		background.setSize(sf::Vector2f(SETTINGS.SCREEN_WIDTH, SETTINGS.SCREEN_HEIGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(0, 0, 0));

		score = 0;
		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);
		scoreText.setString("Score: 0");
		scoreText.setPosition(10.f, 10.f);

		lives = 3;
		livesText.setFont(font);
		livesText.setCharacterSize(24);
		livesText.setFillColor(sf::Color::Red);
		livesText.setString("Lives: " + std::to_string(lives));

		gameObjects.emplace_back(std::make_shared<Platform>(
			sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT / 2.f)));

		auto ball = std::make_shared<Ball>(
			sf::Vector2f(SETTINGS.SCREEN_WIDTH / 2.f,
				SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT - SETTINGS.BALL_SIZE / 2.f));
		ball->AddObserver(weak_from_this());
		gameObjects.emplace_back(ball);

		createBlocks();

		gameOverSound.setBuffer(gameOverSoundBuffer);
	}

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PauseGame();
			}
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{
		for (auto& obj : gameObjects) obj->Update(timeDelta);
		for (auto& block : blocks) block->Update(timeDelta);

		std::shared_ptr<Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
		std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);

		auto isCollision = platform->CheckCollision(ball);

		bool needInverseDirX = false;
		bool needInverseDirY = false;
		bool hasBrokeOneBlock = false;

		blocks.erase(
			std::remove_if(blocks.begin(), blocks.end(),
				[ball, &hasBrokeOneBlock, &needInverseDirX, &needInverseDirY, this](auto block)
				{
					if ((!hasBrokeOneBlock) && block->CheckCollision(ball))
					{
						hasBrokeOneBlock = true;
						if (!block->IsTransparent())
						{
							const auto ballPos = ball->GetPosition();
							const auto blockRect = block->GetRect();
							GetBallInverse(ballPos, blockRect, needInverseDirX, needInverseDirY);
						}
					}
					return block->IsBroken();
				}),
			blocks.end());

		if (needInverseDirX) ball->InvertDirectionX();
		if (needInverseDirY) ball->InvertDirectionY();

		if (shouldLoadNextLevel)
		{
			shouldLoadNextLevel = false;
			LoadNextLevel();
		}

		// обновление бонусов
		for (auto& bonus : bonuses)
		{
			bonus->Update(timeDelta);

			if (!bonus->IsCollected() && bonus->GetRect().intersects(platform->GetRect()))
			{
				bonus->Collect();
				ApplyBonus(bonus->GetType());
			}
		}

		bonuses.erase(
			std::remove_if(bonuses.begin(), bonuses.end(),
				[](auto bonus) { return bonus->IsCollected() || bonus->IsMissed(); }),
			bonuses.end());

		// таймер эффекта
		if (hasActiveEffect)
		{
			bonusTimer += timeDelta;
			if (bonusTimer >= 7.f)
			{
				hasActiveEffect = false;
				bonusTimer = 0.f;
				ResetEffects();
			}
		}
		
	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		window.draw(background);

		for (auto& obj : gameObjects) obj->Draw(window);
		for (auto& block : blocks) block->Draw(window);

		// рисуем бонусы
		for (auto& bonus : bonuses) bonus->Draw(window);
	

		scoreText.setString("Score: " + std::to_string(score));
		scoreText.setOrigin(CalculateTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		livesText.setString("Lives: " + std::to_string(lives));
		livesText.setOrigin(CalculateTextOrigin(livesText, { 1.f, 0.f }));
		livesText.setPosition(SETTINGS.SCREEN_WIDTH - 10.f, 10.f);
		window.draw(livesText);
	}

	void GameStatePlayingData::LoadNextLevel()
	{
		if (currentLevel >= levelLoader.GetLevelCount() - 1)
		{
			Game& game = Application::Instance().GetGame();

			game.UpdateRecord(SETTINGS.PLAYER_NAME, score);
			game.WinGame();
		}
		else
		{
			std::shared_ptr<Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
			std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
			ResetEffects();
			platform->restart();
			ball->restart();

			blocks.clear();
			++currentLevel;
			createBlocks();
		}
	}

	void GameStatePlayingData::createBlocks()
	{
		for (const auto& pair : factories)
		{
			pair.second->ClearCounter();
		}

		auto self = weak_from_this();
		breackableBlocksCount = 0;

		auto level = levelLoader.GetLevel(currentLevel);

		for (auto pairPosBlockType : level.m_blocks)
		{
			auto blockType = pairPosBlockType.second;
			sf::Vector2i pos = pairPosBlockType.first;

			sf::Vector2f position{
				(float)(SETTINGS.BLOCK_SHIFT + SETTINGS.BLOCK_WIDTH / 2.f + pos.x * (SETTINGS.BLOCK_WIDTH + SETTINGS.BLOCK_SHIFT)),
				(float)(pos.y * SETTINGS.BLOCK_HEIGHT + SETTINGS.BLOCK_HEIGHT / 2.f)
			};

			blocks.emplace_back(factories.at(blockType)->CreateBlock(position));
			blocks.back()->AddObserver(self);
		}

		for (const auto& pair : factories)
		{
			breackableBlocksCount += pair.second->GetcreatedBreackableBlocksCount();
		}
	}

	void GameStatePlayingData::GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect,
		bool& needInverseDirX, bool& needInverseDirY)
	{
		if (ballPos.y > blockRect.top + blockRect.height) needInverseDirY = true;
		if (ballPos.x < blockRect.left) needInverseDirX = true;
		if (ballPos.x > blockRect.left + blockRect.width) needInverseDirX = true;
	}

	void GameStatePlayingData::Notify(std::shared_ptr<IObservable> observable)
	{
		if (auto block = std::dynamic_pointer_cast<Block>(observable); block)
		{
			--breackableBlocksCount;

			score += block->GetPoints();
			scoreText.setString("Score: " + std::to_string(score));

			// 40% шанс бонусов
			if (rand() % 100 < 40)
			{
				sf::Vector2f blockPos = block->GetPosition();
				int bonusType = rand() % 3;

				std::shared_ptr<Bonus> bonus;
				switch (bonusType)
				{
				case 0:
					bonus = std::make_shared<FireBallBonus>(blockPos);
					break;
				case 1:
					bonus = std::make_shared<SlowBallBonus>(blockPos);
					break;
				case 2:
					bonus = std::make_shared<BigPlatformBonus>(blockPos);
					break;
				}
				bonuses.push_back(bonus);
			}
			

			if (breackableBlocksCount == 0)
			{
				shouldLoadNextLevel = true;
			}
		}
		else if (auto ball = std::dynamic_pointer_cast<Ball>(observable); ball)
		{
			if (ball->GetPosition().y > gameObjects.front()->GetRect().top)
			{
				gameOverSound.play();

				--lives;

				if (lives > 0)
				{
					std::shared_ptr<Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
					std::shared_ptr<Ball> ballPtr = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
					ResetEffects();
					platform->restart();
					ballPtr->restart();

					livesText.setString("Lives: " + std::to_string(lives));
				}
				else
				{
					Game& game = Application::Instance().GetGame();
					game.UpdateRecord(SETTINGS.PLAYER_NAME, score);
					game.LooseGame();
				}
			}
		}
	}

	// применение и сбор бонусов
	void GameStatePlayingData::ApplyBonus(BonusType type)
	{
		hasActiveEffect = true;
		bonusTimer = 0.f;
		activeBonusType = type;

		std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
		std::shared_ptr<Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);

		switch (type)
		{
		case BonusType::FireBall:
			ball->SetSpeedMultiplier(2.f);
			break;
		case BonusType::SlowBall:
			ball->SetSpeedMultiplier(0.5f);
			break;
		case BonusType::BigPlatform:
			platform->SetScaleMultiplier(1.5f);
			break;
		}
	}

	void GameStatePlayingData::ResetEffects()
	{
		std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
		std::shared_ptr<Platform> platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);

		ball->SetSpeedMultiplier(1.f);
		platform->SetScaleMultiplier(1.f);
	}

}