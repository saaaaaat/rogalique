#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Collidable.h"
#include "IObserver.h"

namespace ArkanoidGame
{
	class Ball final : public GameObject, public Colladiable , public IObservable
	{
	public:
		Ball(const sf::Vector2f& position);
		~Ball() = default;

		void Update(float timeDelta) override;
		void SetSpeedMultiplier(float multiplier) { speedMultiplier = multiplier; }
		float GetSpeedMultiplier() const { return speedMultiplier; }

		void InvertDirectionX();
		void InvertDirectionY();
		void ChangeAngle(float angle);

		bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;

		void restart() override;

	private:
		void OnHit() override;
		float speedMultiplier = 1.f;

		sf::Vector2f direction;
		float lastAngle = 90.f;
	};
}