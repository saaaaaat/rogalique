#include "Application.h"
#include <cstdlib>

namespace ArkanoidGame
{
	Application& Application::Instance()
	{
		static Application instance;
		return instance;
	}
	

	Application::Application() :
		window(sf::VideoMode(SETTINGS.SCREEN_WIDTH, SETTINGS.SCREEN_HEIGHT), SETTINGS.GAME_NAME)//иниц. в списке
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);
	}

	void Application::Run()
	{
		sf::Clock clock;

		while (window.isOpen())
		{
			float start = clock.getElapsedTime().asSeconds();

			game.HandleWindowEvents(window);
			if (!window.isOpen()) break;

			if (game.Update(SETTINGS.TIME_PER_FRAME))
			{
				window.clear();
				game.Draw(window);
				window.display();
			}
			else
			{
				window.close();
			}

			float spent = clock.getElapsedTime().asSeconds() - start;
			if (spent < SETTINGS.TIME_PER_FRAME)
			{
				sf::sleep(sf::seconds(SETTINGS.TIME_PER_FRAME - spent));
			}
		}
	}
}