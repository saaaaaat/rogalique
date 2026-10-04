#include "FireBallBonus.h"

namespace ArkanoidGame
{
	FireBallBonus::FireBallBonus(const sf::Vector2f& position)
		: Bonus(position, sf::Color::Red, BonusType::FireBall)
	{
	}

	void FireBallBonus::Apply()
	{
		
	}
}