#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"
#include <list>

namespace ArkanoidGame
{
	void SetupSprite(sf::Sprite& sprite, float desiredWidth, float desiredHeight, const sf::Texture& texture);
	void ResizeSprite(sf::Sprite& sprite, float desiredWidth, float desiredHeight);
	void CenterSpriteOrigin(sf::Sprite& sprite);

	void SetSpriteColor(sf::Sprite& sprite, const sf::Color& color);

	void PlaceSpriteRandomly(sf::Sprite& sprite, const sf::FloatRect& rect, const std::list<sf::Sprite>& collection);
	sf::Vector2f GetRandomPositionInRect(const sf::Sprite& sprite, const sf::FloatRect& rect);

	void RenderSprite(const sf::Sprite& sprite, sf::RenderWindow& window);

	template<class InputIt>
	void RenderSprites(InputIt first, const InputIt last, sf::RenderWindow& window)
	{
		for (; first != last; ++first)
		{
			RenderSprite(*first, window);
		}
	}

	sf::Vector2f GetVectorFromTo(const sf::Sprite& from, const sf::Sprite& to);
	float GetManhattanDistance(const sf::Sprite& from, const sf::Sprite& to);

	bool IsSpritesOverlap(const sf::Sprite& a, const sf::Sprite& b);

	template<class InputIt>
	bool CheckCollisionsWithList(InputIt first, const InputIt last, const sf::Sprite& sprite)
	{
		for (; first != last; ++first)
		{
			if (IsSpritesOverlap(*first, sprite))
			{
				return true;
			}
		}
		return false;
	}
}