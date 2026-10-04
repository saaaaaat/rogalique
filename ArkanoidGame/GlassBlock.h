#pragma once
#include "Block.h"

namespace ArkanoidGame
{
	class GlassBlock : public Block
	{
	public:
		GlassBlock(const sf::Vector2f& position);

		void OnHit() override;
		bool CheckCollision(std::shared_ptr<Colladiable> collidable) override;
		bool IsTransparent() const override { return true; }
		int GetPoints() const override { return 1; }
	};
}