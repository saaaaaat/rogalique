#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include "randomizer.h"
#include <cmath>

namespace
{
	const std::string TEXTURE_ID = "ball";
}

namespace ArkanoidGame
{
	//создание шарика и задать направление
	Ball::Ball(const sf::Vector2f& position)
		: GameObject(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.BALL_SIZE, SETTINGS.BALL_SIZE)
	{
		const float angle = 90.f;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);
	}
	//обновление позиции и отскоки 
	void Ball::Update(float timeDelta)
	{
		const auto pos = sprite.getPosition() + SETTINGS.BALL_SPEED * speedMultiplier * timeDelta * direction;
		sprite.setPosition(pos);

		if (pos.x - SETTINGS.BALL_SIZE / 2.f <= 0 || pos.x + SETTINGS.BALL_SIZE / 2.f >= SETTINGS.SCREEN_WIDTH)
		{
			direction.x *= -1;
		}

		if (pos.y - SETTINGS.BALL_SIZE / 2.f <= 0 || pos.y + SETTINGS.BALL_SIZE / 2.f >= SETTINGS.SCREEN_HEIGHT)
		{
			direction.y *= -1;
		}
		Emit();
	}

	void Ball::restart()
	{
		GameObject::restart();
		speedMultiplier = 1.f;
		const float angle = 90.f;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = std::sin(pi / 180.f * angle);
	}

	void Ball::InvertDirectionX()
	{
		direction.x *= -1;
	}

	void Ball::InvertDirectionY()
	{
		direction.y *= -1;
	}

	bool Ball::GetCollision(std::shared_ptr<Colladiable> collidable) const
	{
		auto gameObject = std::dynamic_pointer_cast<GameObject>(collidable);
		if (!gameObject) return false;
		return GetRect().intersects(gameObject->GetRect());
	}

	void Ball::OnHit()
	{
		lastAngle += random<float>(-5.f, 5.f);
		ChangeAngle(lastAngle);
	}

	void Ball::ChangeAngle(float angle)
	{
		lastAngle = angle;
		const auto pi = std::acos(-1.f);
		direction.x = (angle / std::fabs(angle)) * std::cos(pi / 180.f * angle);
		direction.y = -1.f * std::fabs(std::sin(pi / 180.f * angle));
	}
}