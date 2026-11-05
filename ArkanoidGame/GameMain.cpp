#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Logger.h"
#include <windows.h>
#include "GameWorld.h"
#include "HealthComponent.h"
#include "RenderSystem.h"

using namespace RogaliqueGame;

namespace {
    void setupLogger() {
        auto logger = std::make_shared<::Engine::Logger>();
        logger->addSink(std::make_shared<::Engine::ConsoleSink>());
        logger->addSink(std::make_shared<::Engine::FileSink>("log.txt"));
        ::Engine::LoggerRegistry::getInstance().registerLogger("global", logger);
        ::Engine::LoggerRegistry::getInstance().setDefaultLogger(logger);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    setupLogger();
    LOG_INFO("Program started");

    const unsigned int W = 1280, H = 720;

    ::Engine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(W, H), "XYZRoguelike"));
    LOG_INFO("Window created: 1280x720");

    ::Engine::ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");
    ::Engine::ResourceSystem::Instance()->LoadTexture("platform", "Resources/Textures/platform.png");
    ::Engine::ResourceSystem::Instance()->LoadMusic("bgm", "Resources/Sounds/AppleEat.wav");
    ::Engine::ResourceSystem::Instance()->LoadFont("default", "Resources/Fonts/Roboto-Regular.ttf");

    if (auto* music = ::Engine::ResourceSystem::Instance()->GetMusic("bgm")) {
        music->setLoop(true); music->setVolume(20.f); music->setPitch(0.6f); music->play();
        LOG_INFO("Background music started");
    }
    else {
        LOG_WARN("Background music not loaded");
    }

    auto developerLevel = std::make_shared<DeveloperLevel>();
    developerLevel->Start();

    // ---- HUD text (screen-space) ----
    auto* hudFont = ::Engine::ResourceSystem::Instance()->GetFont("default");
    sf::Text hudText;
    if (hudFont) hudText.setFont(*hudFont);
    hudText.setCharacterSize(28);
    hudText.setFillColor(sf::Color::White);
    hudText.setOutlineColor(sf::Color::Black);
    hudText.setOutlineThickness(2.f);
    hudText.setPosition(20.f, 20.f);

    // ---- GAME OVER overlay (screen-space) ----
    sf::Text gameOverText;
    if (hudFont) gameOverText.setFont(*hudFont);
    gameOverText.setString("GAME OVER\nPress Enter to restart");
    gameOverText.setCharacterSize(64);
    gameOverText.setFillColor(sf::Color::Red);
    auto b = gameOverText.getLocalBounds();
    gameOverText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    gameOverText.setPosition(W * 0.5f, H * 0.5f);

    // Player finder (for HUD) — resolved once at first Update
    Engine::GameObject* playerGO = nullptr;

    ::Engine::Engine::Instance()->SetUpdateFunction([developerLevel, &playerGO](float dt) {
        developerLevel->Update(dt);

        // Всегда актуализируем указатель на игрока (это безопасно и решает проблему рестарта)
        playerGO = nullptr;
        for (auto* o : ::Engine::GameWorld::Instance()->GetGameObjects()) {
            if (o->GetName() == "Player") {
                playerGO = o;
                break;
            }
        }
        });

    ::Engine::Engine::Instance()->SetRenderFunction(
        [&hudText, &gameOverText, &playerGO, W, H](sf::RenderWindow& window)
        {
            // Save the world-space camera
            auto worldView = window.getView();

            // ---- HUD in screen space ----
            window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)W, (float)H)));
            if (playerGO) {
                if (auto* hp = playerGO->GetComponent<Engine::HealthComponent>()) {
                    std::string s = "HP: " + std::to_string(hp->GetHp()) + "/"
                        + std::to_string(hp->GetMaxHp()) + "\nARMOR: "
                        + std::to_string(hp->GetArmor()) + "/"
                        + std::to_string(hp->GetMaxArmor());
                    hudText.setString(s);
                    window.draw(hudText);
                }
            }

            // ---- GAME OVER overlay ----
            if (::Engine::GameWorld::Instance()->IsPaused()) {
                sf::RectangleShape overlay(sf::Vector2f((float)W, (float)H));
                overlay.setFillColor(sf::Color(0, 0, 0, 200));
                window.draw(overlay);
                window.draw(gameOverText);
            }

            // Restore world camera
            window.setView(worldView);
        });

    ::Engine::Engine::Instance()->Run();
    LOG_INFO("Program exited");
    return 0;
}