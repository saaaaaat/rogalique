#include "Text.h"

namespace ArkanoidGame
{
	sf::Vector2f CalculateTextOrigin(const sf::Text& text, const sf::Vector2f& relativePosition)
	{
		sf::FloatRect bounds = text.getLocalBounds();
		return {
			(bounds.left + bounds.width) * relativePosition.x,
			(bounds.top + bounds.height) * relativePosition.y,
		};
	}

	void DrawTextArray(sf::RenderWindow& window, const std::vector<sf::Text*>& items, float spacing,
		Orientation orientation, Alignment alignment, const sf::Vector2f& position, const sf::Vector2f& origin)
	{
		sf::FloatRect totalBounds;

		for (size_t i = 0; i < items.size(); ++i)
		{
			sf::FloatRect itemBounds = items[i]->getGlobalBounds();
			bool isLast = (i == items.size() - 1);
			float extra = isLast ? 0.f : spacing;

			if (orientation == Orientation::Horizontal)
			{
				totalBounds.width += itemBounds.width + extra;
				totalBounds.height = std::max(totalBounds.height, itemBounds.height);
			}
			else
			{
				totalBounds.width = std::max(totalBounds.width, itemBounds.width);
				totalBounds.height += itemBounds.height + extra;
			}
		}

		totalBounds.left = position.x - origin.x * totalBounds.width;
		totalBounds.top = position.y - origin.y * totalBounds.height;
		sf::Vector2f currentPos = { totalBounds.left, totalBounds.top };

		for (size_t i = 0; i < items.size(); ++i)
		{
			sf::FloatRect itemBounds = items[i]->getGlobalBounds();
			sf::Vector2f itemOrigin;

			if (orientation == Orientation::Horizontal)
			{
				itemOrigin.y = alignment == Alignment::Min ? 0.f : alignment == Alignment::Middle ? 0.5f : 1.f;
				itemOrigin.x = 0.f;
				currentPos.y = totalBounds.top + itemOrigin.y * totalBounds.height;
			}
			else
			{
				itemOrigin.y = 0.f;
				itemOrigin.x = alignment == Alignment::Min ? 0.f : alignment == Alignment::Middle ? 0.5f : 1.f;
				currentPos.x = totalBounds.left + itemOrigin.x * totalBounds.width;
			}

			items[i]->setOrigin(CalculateTextOrigin(*items[i], itemOrigin));
			items[i]->setPosition(currentPos);
			window.draw(*items[i]);

			if (orientation == Orientation::Horizontal)
			{
				currentPos.x += itemBounds.width + spacing;
			}
			else
			{
				currentPos.y += itemBounds.height + spacing;
			}
		}
	}
}