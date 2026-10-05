#include "Menu.h"
#include <cassert>

namespace ArkanoidGame
{
	void Menu::Init(const MenuItem& item)
	{
		rootItem = item;
		SetupMenuItem(rootItem);

		if (!rootItem.childrens.empty())
		{
			HighlightMenuItem(rootItem.childrens.front());
		}
	}

	void Menu::SetupMenuItem(MenuItem& item)
	{
		for (auto& child : item.childrens)
		{
			child.parent = &item;
			SetupMenuItem(child);
		}
	}

	void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem& activeMenu = GetActiveMenu();

		std::vector<sf::Text*> texts;
		for (auto& child : activeMenu.childrens)
		{
			if (child.isEnabled)
			{
				texts.push_back(&child.text);
			}
		}

		DrawTextArray(window, texts, activeMenu.childrenSpacing,
			activeMenu.childrenOrientation, activeMenu.childrenAlignment,
			position, origin);
	}

	void Menu::ActivateSelectedItem()
	{
		if (!selectedItem) return;

		if (selectedItem->onPressCallback)
		{
			selectedItem->onPressCallback(*selectedItem);
			return;
		}

		if (!selectedItem->childrens.empty())
		{
			HighlightMenuItem(selectedItem->childrens.front());
		}
	}

	void Menu::ReturnToParent()
	{
		MenuItem& parent = GetActiveMenu();
		if (&parent != &rootItem)
		{
			HighlightMenuItem(parent);
		}
	}

	void Menu::SelectPreviousItem()
	{
		if (!selectedItem) return;

		MenuItem* parent = selectedItem->parent;
		assert(parent);

		auto it = std::find_if(parent->childrens.begin(), parent->childrens.end(),
			[this](const auto& item) { return selectedItem == &item; });

		if (it != parent->childrens.begin())
		{
			HighlightMenuItem(*std::prev(it));
		}
	}

	void Menu::SelectNextItem()
	{
		if (!selectedItem) return;

		MenuItem* parent = selectedItem->parent;
		assert(parent);

		auto it = std::find_if(parent->childrens.begin(), parent->childrens.end(),
			[this](const auto& item) { return selectedItem == &item; });

		it = std::next(it);
		if (it != parent->childrens.end())
		{
			HighlightMenuItem(*it);
		}
	}

	void Menu::HighlightMenuItem(MenuItem& item)
	{
		assert(&item != &rootItem);

		if (selectedItem == &item) return;
		if (!item.isEnabled) return;

		if (selectedItem)
		{
			selectedItem->text.setFillColor(selectedItem->deselectedColor);
		}

		selectedItem = &item;
		selectedItem->text.setFillColor(selectedItem->selectedColor);
	}

	MenuItem& Menu::GetActiveMenu()
	{
		return selectedItem ? *(selectedItem->parent) : rootItem;
	}
}