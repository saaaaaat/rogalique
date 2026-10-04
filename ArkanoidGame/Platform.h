#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Collidable.h"
#include <algorithm>

namespace ArkanoidGame
{
	class Platform : public GameObject, public Colladiable
	{
	public:
		Platform(const sf::Vector2f& position);
		void Update(float timeDelta) override;

		void SetScaleMultiplier(float multiplier);
		void ResetScale();

		bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;
		void OnHit() override {}
		bool CheckCollision(std::shared_ptr<Colladiable> collidable) override;
		void restart() override;

	private:
		void Move(float speed);
		float scaleMultiplier = 1.f;
		sf::Vector2f baseScale;
	};
}