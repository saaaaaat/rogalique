#pragma once
#include "Bonus.h"

namespace RogaliqueGame
{
	class SlowBallBonus : public Bonus
	{
	public:
		SlowBallBonus(const sf::Vector2f& position);
		void Apply() override;
	};
}
