#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "GameWorld.h"
#include <iostream>

using namespace RogaliqueGame;

int main()
{
   
    ::Engine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "XYZRoguelike"));

 
    ::Engine::ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");
    ::Engine::ResourceSystem::Instance()->LoadTexture("platform", "Resources/Textures/platform.png");
    ::Engine::ResourceSystem::Instance()->LoadMusic("bgm", "Resources/Sounds/AppleEat.wav");
    ::Engine::ResourceSystem::Instance()->LoadTexture("wall_tex", "Resources/Textures/platform.png");

    if (auto music = ::Engine::ResourceSystem::Instance()->GetMusic("bgm"))
    {
        music->setLoop(true);
        music->setVolume(20.f);
        music->setPitch(0.6f);
        music->play();
    }

  
    auto developerLevel = std::make_shared<DeveloperLevel>();
    developerLevel->Start();

    sf::Font font;
    font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf"); 

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER\nPress Enter to restart");
    gameOverText.setCharacterSize(64);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(640.f, 360.f);
    gameOverText.setScale(1.f, -1.f);

  

   
    ::Engine::Engine::Instance()->SetUpdateFunction([developerLevel](float dt) {
        developerLevel->Update(dt);
        });

    ::Engine::Engine::Instance()->SetRenderFunction([&gameOverText](sf::RenderWindow& window) {
        if (::Engine::GameWorld::Instance()->IsPaused())
        {
           
            sf::RectangleShape overlay(sf::Vector2f(1280, 720));
            overlay.setFillColor(sf::Color(0, 0, 0, 255));
         
            window.draw(overlay);
            window.draw(gameOverText);
        }
        });

    
    ::Engine::Engine::Instance()->Run();

    return 0;
}