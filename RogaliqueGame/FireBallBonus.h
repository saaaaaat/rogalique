#pragma once
#include "Bonus.h"

namespace RogaliqueGame
{
	class FireBallBonus : public Bonus
	{
	public:
		FireBallBonus(const sf::Vector2f& position);
		void Apply() override;
	};
}
