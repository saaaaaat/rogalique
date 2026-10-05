
#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"

namespace ArkanoidGame
{
	enum class BonusType
	{
		FireBall,
		SlowBall,
		BigPlatform
	};

	class Bonus : public GameObject
	{
	public:
		Bonus(const sf::Vector2f& position, const sf::Color& color, BonusType type);

		void Update(float timeDelta) override;

		// применить эффект
		virtual void Apply() = 0;

		// тип бонуса
		BonusType GetType() const { return type; }

		// подобран ли
		bool IsCollected() const { return isCollected; }
		void Collect() { isCollected = true; }

		// пролетел ли мимо платформы
		bool IsMissed() const;

	protected:
		BonusType type;
		bool isCollected = false;
		float fallSpeed = 200.f;
	};
}