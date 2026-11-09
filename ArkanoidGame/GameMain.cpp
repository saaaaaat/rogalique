#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Engine.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "GameOrchestrator.h"
#include "PlayerControllerComponent.h"
#include <string>

using namespace RogaliqueGame;
using namespace Engine;

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
    LOG_INFO("Roguelike started");

    const unsigned int W = 1280;
    const unsigned int H = 720;

    RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(W, H), "Roguelike"));

    ResourceSystem::Instance()->LoadFont("default", "Resources/Fonts/Roboto-Regular.ttf");
    const sf::Font* hudFont = ResourceSystem::Instance()->GetFont("default");

    sf::Text hudText;
    if (hudFont) hudText.setFont(*hudFont);
    hudText.setCharacterSize(20);
    hudText.setFillColor(sf::Color::White);
    hudText.setOutlineColor(sf::Color::Black);
    hudText.setOutlineThickness(2.f);
    hudText.setPosition(15.f, 15.f);

    sf::RectangleShape staminaBg(sf::Vector2f(220.f, 14.f));
    staminaBg.setFillColor(sf::Color(40, 40, 40));
    staminaBg.setOutlineColor(sf::Color::White);
    staminaBg.setOutlineThickness(1.f);
    staminaBg.setPosition(15.f, 100.f);

    sf::RectangleShape staminaFg(sf::Vector2f(220.f, 14.f));
    staminaFg.setFillColor(sf::Color(80, 220, 255));
    staminaFg.setPosition(15.f, 100.f);

    sf::Text gameOverText;
    if (hudFont) gameOverText.setFont(*hudFont);
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(3.f);

    sf::Text subText;
    if (hudFont) subText.setFont(*hudFont);
    subText.setString("Press Enter to restart");
    subText.setCharacterSize(32);
    subText.setFillColor(sf::Color::White);
    subText.setOutlineColor(sf::Color::Black);
    subText.setOutlineThickness(2.f);

    auto& orch = GameOrchestrator::Instance();
    orch.StartLevel();

    ::Engine::Engine::Instance()->SetUpdateFunction([&](float dt) {
        orch.Update(dt);
        });

    ::Engine::Engine::Instance()->SetRenderFunction(
        [&](sf::RenderWindow& window)
        {
            sf::View worldView = window.getView();
            sf::View hudView(sf::FloatRect(0.f, 0.f, (float)W, (float)H));
            window.setView(hudView);

            ::Engine::GameObject* player = orch.GetPlayer();
            PlayerControllerComponent* pc = nullptr;
            ::Engine::HealthComponent* ph = nullptr;

            if (player)
            {
                pc = player->GetComponent<PlayerControllerComponent>();
                ph = player->GetComponent<::Engine::HealthComponent>();
            }

            if (pc && ph)
            {
                std::string info =
                    "HP: " + std::to_string(ph->GetHp()) + "/" + std::to_string(ph->GetMaxHp())
                    + "   ARM: " + std::to_string(ph->GetArmor()) + "/" + std::to_string(ph->GetMaxArmor())
                    + "\nStamina: " + std::to_string((int)pc->GetStamina())
                    + "\nInventory:  Q=Clone " + std::string(orch.hasCloneItem ? "[+]" : "[-]")
                    + "   E=Sword " + std::string(orch.hasSwordItem ? "[+]" : "[-]")
                    + "   R=Speed " + std::string(orch.hasSpeedItem ? "[+]" : "[-]")
                    + "   KEY " + std::string(orch.hasKey ? "[+]" : "[-]");
                hudText.setString(info);
                window.draw(hudText);

                float pct = pc->GetStamina() / pc->GetStaminaMax();
                staminaFg.setSize(sf::Vector2f(220.f * pct, 14.f));
                staminaFg.setFillColor(orch.speedBoostTimer > 0.f ? sf::Color(255, 200, 0) : sf::Color(80, 220, 255));
                window.draw(staminaBg);
                window.draw(staminaFg);
            }

            if (orch.IsGameOver())
            {
                sf::RectangleShape overlay(sf::Vector2f((float)W, (float)H));
                overlay.setFillColor(sf::Color(0, 0, 0, 200));
                window.draw(overlay);

                std::string msg = orch.IsWon() ? "YOU ESCAPED!" : "YOU DIED";
                gameOverText.setString(msg);
                gameOverText.setFillColor(orch.IsWon() ? sf::Color::Green : sf::Color::Red);

                sf::FloatRect bounds = gameOverText.getLocalBounds();
                gameOverText.setOrigin(bounds.left + bounds.width * 0.5f, bounds.top + bounds.height * 0.5f);
                gameOverText.setPosition(W * 0.5f, H * 0.5f - 40.f);
                window.draw(gameOverText);

                sf::FloatRect subBounds = subText.getLocalBounds();
                subText.setOrigin(subBounds.left + subBounds.width * 0.5f, subBounds.top + subBounds.height * 0.5f);
                subText.setPosition(W * 0.5f, H * 0.5f + 50.f);
                window.draw(subText);
            }

            window.setView(worldView);
        });

    ::Engine::Engine::Instance()->Run();
    LOG_INFO("Program exited");
    return 0;
}