#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"
#include <iostream>

using namespace RogaliqueGame;

int main()
{

    ::Engine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "XYZRoguelike"));

 
    ::Engine::ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");
    ::Engine::ResourceSystem::Instance()->LoadTexture("platform", "Resources/Textures/platform.png");

 
    ::Engine::ResourceSystem::Instance()->LoadMusic("bgm", "Resources/Sounds/AppleEat.wav");

    if (auto music = ::Engine::ResourceSystem::Instance()->GetMusic("bgm"))
    {
        music->setLoop(true); 
        music->setVolume(20.f);
        music->setPitch(0.6f);
        music->play(); 
        std::cout << "музыка запущена" << std::endl;
    }
    else
    {
        std::cout << "ОШИБКА" << std::endl;
    }

    auto developerLevel = std::make_shared<DeveloperLevel>();
    developerLevel->Start();


    ::Engine::Engine::Instance()->Run();

    return 0;
}