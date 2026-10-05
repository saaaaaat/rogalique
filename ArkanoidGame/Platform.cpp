#include "Platform.h"
#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <algorithm>


namespace
{
	const std::string TEXTURE_ID = "platform";
}

namespace ArkanoidGame
{
	Platform::Platform(const sf::Vector2f& position)
		: GameObject(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.PLATFORM_WIDTH, SETTINGS.PLATFORM_HEIGHT)
	{
		baseScale = sprite.getScale();
		scaleMultiplier = 1.f;
	}

	// управление платформой
	void Platform::Update(float timeDelta)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Move(-timeDelta * SETTINGS.PLATFORM_SPEED);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Move(timeDelta * SETTINGS.PLATFORM_SPEED);
		}
	}

	void Platform::Move(float speed)
	{
		auto position = sprite.getPosition();
		position.x = std::clamp(position.x + speed, SETTINGS.PLATFORM_WIDTH / 2.f, SETTINGS.SCREEN_WIDTH - SETTINGS.PLATFORM_WIDTH / 2.f);
		sprite.setPosition(position);
	}

	void Platform::SetScaleMultiplier(float multiplier)
	{
		scaleMultiplier = multiplier;
		sprite.setScale(
			baseScale.x * scaleMultiplier,
			baseScale.y
		);  // только по X
	}
	void Platform::ResetScale()
	{
		scaleMultiplier = 1.f;
		sprite.setScale(baseScale.x, baseScale.y);
	}

	// проверка касания шарика с платформой
	bool Platform::GetCollision(std::shared_ptr<Colladiable> collidable) const
	{
		auto ball = std::dynamic_pointer_cast<Ball>(collidable);
		if (!ball) return false;

		auto sqr = [](float x) { return x * x; };

		const auto rect = sprite.getGlobalBounds();
		const auto ballPos = ball->GetPosition();
		// шарик слева

		if (ballPos.x < rect.left)
		{
			return sqr(ballPos.x - rect.left) + sqr(ballPos.y - rect.top) < sqr(SETTINGS.BALL_SIZE / 2.0);
		}
		//шарик справа

		if (ballPos.x > rect.left + rect.width)
		{
			return sqr(ballPos.x - rect.left - rect.width) + sqr(ballPos.y - rect.top) < sqr(SETTINGS.BALL_SIZE / 2.0);
		}
		// шарик над
		return std::fabs(ballPos.y - rect.top) <= SETTINGS.BALL_SIZE / 2.0;
	}

	bool Platform::CheckCollision(std::shared_ptr<Colladiable> collidable)
	{
		auto ball = std::dynamic_pointer_cast<Ball>(collidable);
		if (!ball) return false;

		if (GetCollision(ball))
		{
			auto rect = GetRect();
			auto ballPosInPlatform = (ball->GetPosition().x - (rect.left + rect.width / 2)) / (rect.width / 2);
			ball->ChangeAngle(90 - 20 * ballPosInPlatform);
			return true;
		}
		return false;
	}
	void Platform::restart()
	{
		GameObject::restart();
		ResetScale();
	}
}