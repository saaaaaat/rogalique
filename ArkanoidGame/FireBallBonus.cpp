#include "FireBallBonus.h"

namespace RogaliqueGame
{
	FireBallBonus::FireBallBonus(const sf::Vector2f& position)
		: Bonus(position, sf::Color::Red, BonusType::FireBall)
	{
	}

	void FireBallBonus::Apply()
	{
		
	}
}