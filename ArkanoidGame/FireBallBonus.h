#pragma once
#include "Bonus.h"

namespace ArkanoidGame
{
	class FireBallBonus : public Bonus
	{
	public:
		FireBallBonus(const sf::Vector2f& position);
		void Apply() override;
	};
}
