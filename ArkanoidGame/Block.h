#pragma once
#include "Ball.h"
#include "GameObject.h"
#include "Collidable.h"
#include "IDelayedAction.h"
#include "IObserver.h"

namespace ArkanoidGame
{
	class Block : public GameObject, public Colladiable , public IObservable
	{
	protected:
		virtual void OnHit() override;
		int hitCount = 1;

	public:
		Block(const sf::Vector2f& position, const sf::Color& color = sf::Color(150, 200, 255));
		virtual ~Block();

		bool GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		void Update(float timeDelta) override;
		bool IsBroken();
		virtual bool IsTransparent() const { return false; }
		virtual bool IsUnbreakable() const { return false; }
		virtual int GetPoints() const { return 1; }
	};

	class SmoothDestroyableBlock : public Block, public IDelayedAction
	{
	protected:
		void OnHit() override;
		sf::Color color;

	public:
		// позиция и цвет стандартного блока
		SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color = sf::Color(150, 200, 255));
		~SmoothDestroyableBlock() = default;

		void Update(float timeDelta) override;
		bool GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
		void FinalAction() override;
		void EachTickAction(float deltaTime) override;
		
	};

	class UnbreackableBlock : public Block
	{
	public:
		UnbreackableBlock(const sf::Vector2f& position);
		void OnHit() override;
		void Update(float) override {}
		bool IsUnbreakable() const override { return true; }
		int GetPoints() const override { return 0; }
	};
}