#pragma once
#include "Bonus.h"

namespace ArkanoidGame
{
	class BigPlatformBonus : public Bonus
	{
	public:
		BigPlatformBonus(const sf::Vector2f& position);
		void Apply() override;
	};
}
