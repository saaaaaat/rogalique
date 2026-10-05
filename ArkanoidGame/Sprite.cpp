#include "Sprite.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
	void SetupSprite(sf::Sprite& sprite, float width, float height, const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		CenterSpriteOrigin(sprite);
		ResizeSprite(sprite, width, height);
	}

	void RenderSprite(const sf::Sprite& sprite, sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	sf::Vector2f GetRandomPositionInRect(const sf::Sprite& sprite, const sf::FloatRect& rect)
	{
		sf::Vector2f result;
		float spriteW = sprite.getGlobalBounds().width;
		float spriteH = sprite.getGlobalBounds().height;

		result.x = rand() / (float)RAND_MAX * (rect.width - 2 * spriteW) + rect.left + spriteW;
		result.y = rand() / (float)RAND_MAX * (rect.height - 2 * spriteH) + rect.top + spriteH;
		return result;
	}

	void ResizeSprite(sf::Sprite& sprite, float width, float height)
	{
		sf::FloatRect bounds = sprite.getGlobalBounds();
		sf::Vector2f scale = { width / bounds.width, height / bounds.height };
		sprite.setScale(scale);
	}

	void CenterSpriteOrigin(sf::Sprite& sprite)
	{
		sf::FloatRect bounds = sprite.getGlobalBounds();
		sprite.setOrigin(0.5f * bounds.width, 0.5f * bounds.height);
	}

	void SetSpriteColor(sf::Sprite& sprite, const sf::Color& color)
	{
		sprite.setColor(color);
	}

	void PlaceSpriteRandomly(sf::Sprite& sprite, const sf::FloatRect& area, const std::list<sf::Sprite>& obstacles)
	{
		bool hasCollision = true;
		while (hasCollision)
		{
			sf::Vector2f pos = GetRandomPositionInRect(sprite, area);
			sprite.setPosition(pos);
			hasCollision = CheckCollisionsWithList(obstacles.begin(), obstacles.end(), sprite);
		}
	}

	bool IsSpritesOverlap(const sf::Sprite& a, const sf::Sprite& b)
	{
		sf::Vector2f diff = a.getPosition() - b.getPosition();
		float distance = sqrtf(diff.x * diff.x + diff.y * diff.y);
		float sumRadii = (a.getGlobalBounds().width + b.getGlobalBounds().width) * 0.5f;
		return distance < sumRadii;
	}

	sf::Vector2f GetVectorFromTo(const sf::Sprite& from, const sf::Sprite& to)
	{
		return to.getPosition() - from.getPosition();
	}

	float GetManhattanDistance(const sf::Sprite& from, const sf::Sprite& to)
	{
		sf::Vector2f diff = to.getPosition() - from.getPosition();
		return std::fabs(diff.x) + std::fabs(diff.y);
	}
}