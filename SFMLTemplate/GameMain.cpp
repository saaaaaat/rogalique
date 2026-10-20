// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//#include <windows.h>
//#include <iostream>
const std::string RESOURCES_PATH = "Resources/";

int main()
{
	// Выделяем консоль и перенаправляем вывод
	/*if (AllocConsole())
	{
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout); // Перенаправляем stdout в консоль
		freopen_s(&fp, "CONOUT$", "w", stderr); // Перенаправляем stderr в консоль
	}
	else
	{
		std::cerr << "Не удалось выделить консоль." << std::endl;
	}*/

	RogaliqueGame RogaliqueGame;
	RogaliqueGame.Initialize();
	RogaliqueGame.Run();
	sf::RenderWindow window(sf::VideoMode(330, 400), "SFML works!");

	sf::Texture logo;
	if (!logo.loadFromFile(RESOURCES_PATH + "xyz-logo.png"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite logo_sprite(logo);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(logo_sprite);
		window.display();
	}

	return 0;
}
