#include "pch.h"
#include "Engine.h"
#include "GameWorld.h"
#include "RenderSystem.h"

namespace Engine
{
	Engine* Engine::Instance()
	{
		static Engine instance;
		return &instance;
	}

	Engine::Engine()
	{
		srand((unsigned int)time(nullptr));
	}

	void Engine::SetUpdateFunction(std::function<void(float)> updateFunc)
	{
		customUpdateFunc = updateFunc;
	}

	void Engine::SetRenderFunction(std::function<void(sf::RenderWindow&)> renderFunc)
	{
		customRenderFunc = renderFunc;
	}

	void Engine::Run()
	{
		sf::Clock gameClock;
		sf::Event event;

		while (RenderSystem::Instance()->GetMainWindow().isOpen())
		{
			sf::Time dt = gameClock.restart();
			float deltaTime = dt.asSeconds();

			while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					RenderSystem::Instance()->GetMainWindow().close();
			}

			if (!RenderSystem::Instance()->GetMainWindow().isOpen()) break;

			RenderSystem::Instance()->GetMainWindow().clear();

			if (customUpdateFunc)
			{
				customUpdateFunc(deltaTime);
			}

			
			if (!GameWorld::Instance()->IsPaused())
			{
				GameWorld::Instance()->Update(deltaTime);
				GameWorld::Instance()->FixedUpdate(deltaTime);
			}
			GameWorld::Instance()->Render();
			GameWorld::Instance()->LateUpdate();

			if (customRenderFunc)
			{
				customRenderFunc(RenderSystem::Instance()->GetMainWindow());
			}

			RenderSystem::Instance()->GetMainWindow().display();
		}
	}
}