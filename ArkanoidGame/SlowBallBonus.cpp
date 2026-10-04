#include "SlowBallBonus.h"

namespace ArkanoidGame
{
	SlowBallBonus::SlowBallBonus(const sf::Vector2f& position)
		: Bonus(position, sf::Color::Blue, BonusType::SlowBall)
	{
	}

	void SlowBallBonus::Apply()
	{
		
	}
}