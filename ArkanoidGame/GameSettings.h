#pragma once
#include <string>

namespace ArkanoidGame
{
	class GameWorld
	{
	private:
		GameWorld() = default;

	public:
		static GameWorld& Instance()
		{
			static GameWorld gameWorld;
			return gameWorld;
		}

		const std::string RESOURCES_PATH = "Resources/";
		const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
		const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
		const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";
		const std::string LEVELS_CONFIG_PATH = RESOURCES_PATH + "levels.config";

		const float ACCELERATION = 10.f;
		const int MAX_APPLES = 80;
		const unsigned int SCREEN_WIDTH = 800;
		const unsigned int SCREEN_HEIGHT = 600;
		const float TIME_PER_FRAME = 1.f / 60.f;

		const unsigned int BALL_SIZE = 20;
		const unsigned int BALL_SPEED = 400;

		const unsigned int PLATFORM_WIDTH = 160;
		const unsigned int PLATFORM_HEIGHT = 20;
		const float PLATFORM_SPEED = 300.f;

		const unsigned int BLOCKS_COUNT_ROWS = 4;
		const unsigned int BLOCKS_COUNT_IN_ROW = 15;
		const unsigned int BLOCK_SHIFT = 5;
		const unsigned int BLOCK_WIDTH = (SCREEN_WIDTH - (BLOCKS_COUNT_IN_ROW + 1) * BLOCK_SHIFT) / BLOCKS_COUNT_IN_ROW;
		const unsigned int BLOCK_HEIGHT = 20;

		const int POINTS_SIMPLE_BLOCK = 10;      // обычный
		const int POINTS_DURABLE_BLOCK = 30;     // 3 удара
		const int POINTS_GLASS_BLOCK = 20;       // стеклянный
		const int POINTS_UNBREAKABLE_BLOCK = 0;  // неразрушаемый (не даёт очков)

		const int MAX_RECORDS_TABLE_SIZE = 5;
		const char* PLAYER_NAME = "Player";

		const std::string GAME_NAME = "ArkanoidGame";
		const float BREAK_DELAY = 1.f;
	};
}

#define SETTINGS GameWorld::Instance()