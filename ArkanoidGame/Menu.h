#pragma once
#include "SFML/Graphics.hpp"
#include "Text.h"
#include <functional>

namespace ArkanoidGame
{
	struct MenuItem
	{
		sf::Text text;
		sf::Text hintText;

		Orientation childrenOrientation = Orientation::Vertical;
		Alignment childrenAlignment = Alignment::Min;
		float childrenSpacing = 0.f;

		sf::Color selectedColor = sf::Color::Yellow;
		sf::Color deselectedColor = sf::Color::White;

		bool isEnabled = true;
		std::vector<MenuItem> childrens;

		std::function<void(MenuItem& item)> onPressCallback;

		MenuItem* parent = nullptr;// иницилизация указателя
	};

	class Menu
	{
	public:
		void Init(const MenuItem& item);
		void Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);

		void ActivateSelectedItem();
		void ReturnToParent();

		void SelectPreviousItem();
		void SelectNextItem();

		MenuItem& GetActiveMenu();

	private:
		void SetupMenuItem(MenuItem& item);
		void HighlightMenuItem(MenuItem& item);

	private:
		MenuItem rootItem;
		MenuItem* selectedItem = nullptr;
	};
}