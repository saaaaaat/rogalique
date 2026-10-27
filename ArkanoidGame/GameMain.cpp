#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"
#include "Logger.h" 
#include <windows.h>
#include <iostream>
#include <clocale>

using namespace RogaliqueGame;


void setupLogger() {
    auto logger = std::make_shared<::Engine::Logger>();
    logger->addSink(std::make_shared<::Engine::ConsoleSink>());
    logger->addSink(std::make_shared<::Engine::FileSink>("log.txt"));

    ::Engine::LoggerRegistry::getInstance().registerLogger("global", logger);
    ::Engine::LoggerRegistry::getInstance().setDefaultLogger(logger);
}

int main()
{
   
    SetConsoleOutputCP(CP_UTF8);
  
    setupLogger();

    LOG_INFO("Программа запущена!");


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
        std::cout << "Музыка запущена!" << std::endl;
    }
    else
    {
        std::cout << "ОШИБКА: Музыка не загружена!" << std::endl;
    }


     auto developerLevel = std::make_shared<DeveloperLevel>();
     developerLevel->Start();

   
    ::Engine::Engine::Instance()->Run();

    return 0;
}