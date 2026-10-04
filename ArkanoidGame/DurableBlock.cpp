#include "DurableBlock.h"
#include "Sprite.h"

namespace ArkanoidGame
{
	DurableBlock::DurableBlock(const sf::Vector2f& position)
		: Block(position, sf::Color::Red)   //  красный
	{
		hitPoints = 3;
	}

	void DurableBlock::OnHit()
	{
		--hitPoints;

		if (hitPoints == 2)
		{
			sprite.setColor(sf::Color(255, 69, 0));   // оранжевый
		}
		else if (hitPoints == 1)
		{
			sprite.setColor(sf::Color(255,207,64)); // жёлтый
		}
		else if (hitPoints <= 0)
		{
			hitCount = 0; // разрушение
			Emit();
		}
	}

	void DurableBlock::Update(float timeDelta)
	{
		
	}
}