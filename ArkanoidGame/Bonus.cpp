#include "Bonus.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
	Bonus::Bonus(const sf::Vector2f& position, const sf::Color& color, BonusType type)
		: GameObject(SETTINGS.TEXTURES_PATH + "ball.png", position, SETTINGS.BALL_SIZE, SETTINGS.BALL_SIZE)
		, type(type)
	{
		sprite.setColor(color);
	}

	void Bonus::Update(float timeDelta)
	{
		sf::Vector2f pos = sprite.getPosition();
		pos.y += fallSpeed * timeDelta;
		sprite.setPosition(pos);
	}

	bool Bonus::IsMissed() const
	{
		return sprite.getPosition().y > SETTINGS.SCREEN_HEIGHT;
	}
}